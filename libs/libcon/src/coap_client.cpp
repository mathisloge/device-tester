#include <cstdlib>
#include <mutex>
#include <shared_mutex>
#include <asio.hpp>
#include <coap3/coap.h>
#include <libcon/coap_client.hpp>
#include <spdlog/spdlog.h>
#include "coap_state.hpp"
#include "manager_impl.hpp"
namespace dev::con
{

class CoapClient::Impl final
{
    // clang-format off
    struct CoapCtxDeleter { void operator()(coap_context_t *ctx) { if (ctx) { coap_free_context(ctx); } } };
    struct CoapSessionDeleter { void operator()(coap_session_t *sess) { if (sess) { coap_session_release(sess); } } };
    // clang-format on

    static coap_response_t CoapMessageCallback(coap_session_t *session,
                                               const coap_pdu_t *sent,
                                               const coap_pdu_t *received,
                                               const coap_mid_t mid)
    {
        Impl *impl = reinterpret_cast<Impl *>(coap_session_get_app_data(session));
        if (impl)
        {
            const auto ret_code = impl->messageCallback(session, sent, received, mid);
            spdlog::info("CODE {}", ret_code);
            return ret_code;
        }
        spdlog::error("couldn't find coap impl by session");
        return COAP_RESPONSE_OK;
    }
    static void FreeLargeDataCallback(coap_session_t *session, void *app_ptr)
    {
        Impl *impl = reinterpret_cast<Impl *>(coap_session_get_app_data(session));
        if (!impl)
        {
            spdlog::error("FreeLargeData; couldn't find coap impl by session");
            return;
        }
        impl->freeLargeData(reinterpret_cast<size_t>(app_ptr));
    }

  public:
    Impl(Manager &manager)
        : strand_{asio::make_strand(manager.impl().ctx())}
        , session_{nullptr, CoapSessionDeleter{}}
        , large_data_{}
    {

        ensure_coap();
        ctx_ = std::unique_ptr<coap_context_t, CoapCtxDeleter>(coap_new_context(nullptr), CoapCtxDeleter{});
        coap_context_set_block_mode(ctx_.get(), COAP_BLOCK_USE_LIBCOAP);
        coap_register_option(ctx_.get(), COAP_OPTION_BLOCK2);
        coap_register_response_handler(ctx_.get(), &Impl::CoapMessageCallback);

        do_work_ = true;
        asio::post(strand_, std::bind(&Impl::doIO, this));

        opts_.server_uri = "fe80::7ae3:6dff:fe09:8188";
        opts_.server_port = COAP_DEFAULT_PORT;
        createSession();
    }

    ~Impl()
    {
        std::unique_lock<std::shared_mutex> l{coap_mtx_};
        do_work_ = false;
    }

    void setOptions(const Options &opts)
    {
        opts_ = opts;
        createSession();
    }

    void sendRequest(const std::string_view path,
                     const Method method,
                     const RequestId id,
                     const std::span<uint8_t> data)
    {
        coap_optlist_t *optlist = nullptr;
        {
            constexpr int BUFSIZE = 100;
            unsigned char _buf[BUFSIZE];
            unsigned char *buf = _buf;
            size_t buflen;

            auto res = coap_split_path(reinterpret_cast<const uint8_t *>(path.data()), path.size(), buf, &buflen);
            while (res--)
            {
                coap_insert_optlist(&optlist,
                                    coap_new_optlist(COAP_OPTION_URI_PATH, coap_opt_length(buf), coap_opt_value(buf)));

                buf += coap_opt_size(buf);
            }
        }
        const coap_pdu_code_t req_code = [method]() -> coap_pdu_code_t {
            switch (method)
            {
            case Method::m_get:
                return COAP_REQUEST_CODE_GET;
            case Method::m_post:
                return COAP_REQUEST_CODE_POST;
            case Method::m_put:
                return COAP_REQUEST_CODE_PUT;
            case Method::m_delete:
                return COAP_REQUEST_CODE_DELETE;

            default:
                return COAP_EMPTY_CODE;
            }
        }();

        std::shared_lock<std::shared_mutex> l{coap_mtx_};
        coap_pdu_t *pdu = coap_new_pdu(COAP_MESSAGE_CON, req_code, session_.get());

        uint8_t token[8];
        size_t tokenlen;
        coap_session_new_token(session_.get(), &tokenlen, token);
        track_new_token(tokenlen, token, id);
        if (!coap_add_token(pdu, tokenlen, token))
        {
            spdlog::error("cannot add token to request");
        }
        if (data.size() > 0)
        {
            std::vector<uint8_t> tmp_mem;
            tmp_mem.assign(data.begin(), data.end());

            {
                std::unique_lock l{large_data_.mtx};
                auto it = large_data_.buffer.emplace(large_data_.id_counter++, std::move(tmp_mem));

                /* Let the underlying libcoap decide how this data should be sent */
                auto &buffer_data = it.first->second;
                coap_add_data_large_request(session_.get(),
                                            pdu,
                                            buffer_data.size(),
                                            &buffer_data[0],
                                            &Impl::FreeLargeDataCallback,
                                            reinterpret_cast<void *>(it.first->first));
            }
        }

        coap_add_optlist_pdu(pdu, &optlist);

        coap_show_pdu(LOG_INFO, pdu);
        coap_send(session_.get(), pdu);
    }

  private:
    void doIO()
    {
        if (!do_work_)
            return;
        {
            std::shared_lock<std::shared_mutex> l{coap_mtx_};
            const auto result = coap_io_process(ctx_.get(), 250);
        }
        if (do_work_)
            asio::post(strand_, std::bind(&Impl::doIO, this));
    }

    void createSession()
    {
        coap_address_t *bind_addr = nullptr;

        asio::io_context io_ctx;
        asio::ip::udp::resolver resolver{io_ctx};
        asio::error_code error;
        const auto endpoints = resolver.resolve(opts_.server_uri, std::to_string(opts_.server_port), error);

        if (endpoints.size() == 0)
            return;
        const auto &ep = endpoints.begin()->endpoint();
        is_mcast_ = ep.address().is_multicast();
        const auto ainfo = ep.data();
        coap_address_t dst;
        memcpy(&dst.addr, ainfo, ep.size());
        dst.size = ep.size();
        auto new_session = coap_new_client_session(ctx_.get(), bind_addr, &dst, COAP_PROTO_UDP);
        {
            std::unique_lock<std::shared_mutex> l{coap_mtx_};

            coap_session_set_app_data(new_session, this); // save the reference
            session_ = std::unique_ptr<coap_session_t, CoapSessionDeleter>(new_session, CoapSessionDeleter{});
        }
    }

    coap_response_t messageCallback(coap_session_t *session,
                                    const coap_pdu_t *sent,
                                    const coap_pdu_t *received,
                                    const coap_mid_t mid)
    {
        const coap_pdu_code_t rcv_code = coap_pdu_get_code(received);
        const coap_pdu_type_t rcv_type = coap_pdu_get_type(received);
        const coap_bin_const_t token = coap_pdu_get_token(received);
        spdlog::info("process incoming {}.{} response", COAP_RESPONSE_CLASS(rcv_code), rcv_code & 0x1F);
        coap_show_pdu(LOG_INFO, received);

        /* check if this is a response to our original request */
        const auto [same_token, req_id] = track_check_token(&token);
        if (!same_token)
        {
            /* drop if this was just some message, or send RST in case of notification */
            if (!sent && (rcv_type == COAP_MESSAGE_CON || rcv_type == COAP_MESSAGE_NON))
            {
                spdlog::info("!track COAP_RESPONSE_FAIL");
                /* Cause a CoAP RST to be sent */
                return COAP_RESPONSE_FAIL;
            }
            spdlog::info("!track COAP_RESPONSE_OK");
            return COAP_RESPONSE_OK;
        }
        if (rcv_type == COAP_MESSAGE_RST)
        {
            spdlog::info("got RST");
            return COAP_RESPONSE_OK;
        }
        /* output the received data, if any */
        if (COAP_RESPONSE_CLASS(rcv_code) == 2)
        {
            coap_opt_t *block_opt;
            coap_opt_iterator_t opt_iter;
            /* set obs timer if we have successfully subscribed a resource */
            if (doing_observe && !obs_started && coap_check_option(received, COAP_OPTION_OBSERVE, &opt_iter))
            {
                spdlog::info("observation relationship established");
                // coap_log(LOG_DEBUG, "observation relationship established, set timeout to %d\n", obs_seconds);
                // obs_started = 1;
                // obs_ms = obs_seconds * 1000;
                // obs_ms_reset = 1;
            }
            size_t len;
            const uint8_t *databuf;
            size_t offset;
            size_t total;
            if (coap_get_data_large(received, &len, &databuf, &offset, &total))
            {
                rx_signal_(req_id, std::span<const uint8_t>(databuf, len));
                // append_to_output(databuf, len);
                // if ((len + offset == total) && add_nl)
                //     append_to_output((const uint8_t *)"\n", 1);
            }
            else
                rx_signal_(req_id, {});

            /* Check if Block2 option is set */
            block_opt = coap_check_option(received, COAP_OPTION_BLOCK2, &opt_iter);
            if (/*!single_block_requested &&*/ block_opt)
            { /* handle Block2 */

                /* TODO: check if we are looking at the correct block number */
                if (coap_opt_block_num(block_opt) == 0)
                {
                    /* See if observe is set in first response */
                    // ready = doing_observe ? coap_check_option(received, COAP_OPTION_OBSERVE, &opt_iter) == NULL : 1;
                }
                if (COAP_OPT_BLOCK_MORE(block_opt))
                {
                    // wait_ms = wait_seconds * 1000;
                    // wait_ms_reset = 1;
                    // doing_getting_block = 1;
                }
                else
                {
                    // doing_getting_block = 0;
                    track_flush_token(&token);
                }
                return COAP_RESPONSE_OK;
            }
        }
        else
        { /* no 2.05 */
            /* check if an error was signaled and output payload if so */
            if (COAP_RESPONSE_CLASS(rcv_code) >= 4)
            {
                fprintf(stderr, "%d.%02d", COAP_RESPONSE_CLASS(rcv_code), rcv_code & 0x1F);
                size_t len;
                const uint8_t *databuf;
                size_t offset;
                size_t total;
                if (coap_get_data_large(received, &len, &databuf, &offset, &total))
                {
                    fprintf(stderr, " ");
                    while (len--)
                    {
                        fprintf(stderr, "%c", isprint(*databuf) ? *databuf : '.');
                        databuf++;
                    }
                }
                fprintf(stderr, "\n");
            }
        }
        if (!is_mcast_)
            track_flush_token(&token);

        return COAP_RESPONSE_OK;
    }

    //! TODO REFACTOR THOSE FUNCTIONS
    void track_new_token(size_t tokenlen, uint8_t *token, RequestId id)
    {
        track_token *new_list =
            (track_token *)realloc(tracked_tokens, (tracked_tokens_count + 1) * sizeof(tracked_tokens[0]));
        if (!new_list)
        {
            coap_log(LOG_INFO, "Unable to track new token\n");
            return;
        }
        tracked_tokens = new_list;
        tracked_tokens[tracked_tokens_count].token = coap_new_binary(tokenlen);
        if (!tracked_tokens[tracked_tokens_count].token)
            return;
        memcpy(tracked_tokens[tracked_tokens_count].token->s, token, tokenlen);
        tracked_tokens[tracked_tokens_count].observe = doing_observe;
        tracked_tokens[tracked_tokens_count].id = id;
        tracked_tokens_count++;
    }

    std::pair<bool, RequestId> track_check_token(const coap_bin_const_t *token) const
    {
        for (size_t i = 0; i < tracked_tokens_count; i++)
        {
            if (coap_binary_equal(token, tracked_tokens[i].token))
            {
                return {true, tracked_tokens[i].id};
            }
        }
        return {false, 0};
    }

    void track_flush_token(const coap_bin_const_t *token)
    {
        for (size_t i = 0; i < tracked_tokens_count; i++)
        {
            if (coap_binary_equal(token, tracked_tokens[i].token))
            {
                if (!tracked_tokens[i].observe || !obs_started)
                {
                    /* Only remove if not Observing */
                    coap_delete_binary(tracked_tokens[i].token);
                    if (tracked_tokens_count - i > 1)
                    {
                        memmove(&tracked_tokens[i],
                                &tracked_tokens[i + 1],
                                (tracked_tokens_count - i - 1) * sizeof(tracked_tokens[0]));
                    }
                    tracked_tokens_count--;
                }
                break;
            }
        }
    }

    void freeLargeData(const size_t id)
    {
        std::unique_lock l{large_data_.mtx};
        auto it = large_data_.buffer.find(id);
        if (it != large_data_.buffer.end())
            large_data_.buffer.erase(it);
    }

  private:
    bool do_work_;
    std::shared_mutex coap_mtx_;
    asio::strand<asio::any_io_executor> strand_;
    std::unique_ptr<coap_context_t, CoapCtxDeleter> ctx_;
    std::unique_ptr<coap_session_t, CoapSessionDeleter> session_;
    struct
    {
        std::mutex mtx;
        size_t id_counter;
        std::map<size_t, std::vector<uint8_t>> buffer;
    } large_data_;

    struct track_token
    {
        RequestId id;
        coap_binary_t *token;
        int observe;
    };
    track_token *tracked_tokens = NULL;
    size_t tracked_tokens_count = 0;
    int obs_started = 0;
    int doing_observe = 0;
    bool is_mcast_;

  public:
    Options opts_;
    RxSig rx_signal_;
};

void CoapClient::DiscoverServers()
{
    ensure_coap();
}

int CoapClient::DefaultPort()
{
    return COAP_DEFAULT_PORT;
}
int CoapClient::DefaultSecurePort()
{
    return COAPS_DEFAULT_PORT;
}

CoapClient::CoapClient(Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

CoapClient::~CoapClient()
{}

void CoapClient::setOptions(const Options &opts)
{
    impl_->setOptions(opts);
}

const CoapClient::Options &CoapClient::options() const
{
    return impl_->opts_;
}

void CoapClient::getWellKnown(const std::string path)
{
    impl_->sendRequest(path, Method::m_get, kRequestIdWellKnown, {});
}

void CoapClient::makeRequest(const Method method,
                             const std::string &path,
                             const RequestId id,
                             const std::span<uint8_t> data)
{
    impl_->sendRequest(path, method, id, data);
}

std::string CoapClient::generateReadableName() const
{
    return fmt::format("[COAP-Client] {}", impl_->opts_.server_uri);
}

sig::connection CoapClient::connectOnReceive(const RxSig::slot_type &sub)
{
    return impl_->rx_signal_.connect(sub);
}

} // namespace dev::con
