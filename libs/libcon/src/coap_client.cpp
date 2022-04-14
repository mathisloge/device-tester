#include <asio.hpp>
#include <coap3/coap.h>
#include <libcon/coap_client.hpp>
#include <spdlog/spdlog.h>
#include "coap_state.hpp"
namespace dev::con
{
// clang-format off
struct CoapCtxDeleter { void operator()(coap_context_t *ctx) { spdlog::info("COAP DELTE CTX"); if (ctx) coap_free_context(ctx); } };
struct CoapSessionDeleter { void operator()(coap_session_t *ctx) { spdlog::info("COAP DELTE SESSION"); if (ctx) coap_session_release(ctx); } };
// clang-format on

class CoapClient::Impl final
{
  public:
    Impl()
        : session_{nullptr, CoapSessionDeleter{}}
    {
        ensure_coap();
        ctx_ = std::unique_ptr<coap_context_t, CoapCtxDeleter>(coap_new_context(nullptr), CoapCtxDeleter{});
        coap_register_option(ctx_.get(), COAP_OPTION_BLOCK2);
        
        opts_.server_uri = "fe80::7ae3:6dff:fe09:8188";
        opts_.server_port = COAP_DEFAULT_PORT;

        createSession();
    }

    ~Impl()
    {}

    void sendTestRequest(const std::string_view path)
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

        coap_pdu_t *pdu = coap_new_pdu(COAP_MESSAGE_CON, COAP_REQUEST_CODE_GET, session_.get());

        uint8_t token[8];
        size_t tokenlen;
        coap_session_new_token(session_.get(), &tokenlen, token);
        // track_new_token(tokenlen, token);
        if (!coap_add_token(pdu, tokenlen, token))
        {
            spdlog::error("cannot add token to request");
        }

        coap_add_optlist_pdu(pdu, &optlist);

        coap_show_pdu(LOG_INFO, pdu);
        coap_send(session_.get(), pdu);
    }

  private:
    void createSession()
    {
        coap_address_t *bind_addr = nullptr;

        asio::io_context io_ctx;
        asio::ip::udp::resolver resolver{io_ctx};
        asio::error_code error;
        const auto endpoints = resolver.resolve(opts_.server_uri, std::to_string(opts_.server_port), error);

        if (endpoints.size() == 0)
            return;
        const auto ainfo = endpoints.begin()->endpoint().data();
        coap_address_t dst;
        memcpy(&dst.addr, ainfo, endpoints.begin()->endpoint().size());
        dst.size = endpoints.begin()->endpoint().size();
        auto new_session = coap_new_client_session(ctx_.get(), bind_addr, &dst, COAP_PROTO_UDP);
        session_ = std::unique_ptr<coap_session_t, CoapSessionDeleter>(new_session, CoapSessionDeleter{});
    }

  private:
    std::unique_ptr<coap_context_t, CoapCtxDeleter> ctx_;
    std::unique_ptr<coap_session_t, CoapSessionDeleter> session_;

  public:
    Options opts_;
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

CoapClient::CoapClient()
    : impl_{std::make_unique<Impl>()}
{}

CoapClient::~CoapClient()
{}

void CoapClient::applyOptions(const Options &opts)
{
    impl_->opts_ = opts;
}

void CoapClient::getWellKnown(const std::string path)
{
    impl_->sendTestRequest(path);
}

std::string CoapClient::generateReadableName() const
{
    return "COAP CLIENT TODO";
}

} // namespace dev::con
