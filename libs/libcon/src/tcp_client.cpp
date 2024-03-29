#include "libcon/tcp_client.hpp"
#include <atomic>
#include <mutex>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "basic_client.hpp"
#include "manager_impl.hpp"
namespace dev::con
{

class TcpClient::Impl final : public BasicClient
{
  public:
    using tcp = asio::ip::tcp;

  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl().ctx()}
        , opts_{}
        , socket_{strand_}
        , resolver_{strand_}
        , should_run_{true}
        , write_in_progress_{false}
    {}
    ~Impl()
    {
        disconnect();
    }

    void setOptions(const Options &opts)
    {
        opts_ = opts;
    }
    bool isConnected() const
    {
        return socket_.is_open();
    }
    void connect()
    {
        asio::io_context resolver_ctx;
        tcp::resolver resolver{resolver_ctx};
        asio::error_code err_code;
        spdlog::debug("Connecting to: {} service: {}", opts_.server, opts_.service);
        resolver_.async_resolve(
            opts_.server, opts_.service, [this](const asio::error_code &ec, tcp::resolver::results_type endpoints) {
                if (!ec)
                {
                    asio::async_connect(
                        socket_,
                        endpoints,
                        std::bind(&Impl::handleConnect, this, std::placeholders::_1, std::placeholders::_2));
                }
                else
                {
                    spdlog::error("ERROR: {}", ec.message());
                    disconnect();
                }
            });
    }

    void disconnect()
    {
        should_run_ = false;
        // silent disconnect
        {
            asio::error_code e;
            socket_.close(e);
        }
        // WORKAROUND: wait a short amount of time, to actually close the socket.
        // https://github.com/mathisloge/device-tester/issues/1
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

  private:
    void handleConnect(const std::error_code &ec, const tcp::endpoint &endpoint)
    {
        if (!ec)
        {
            spdlog::debug("[tcp-client] {}:{}", endpoint.address().to_string(), endpoint.port());
            connection_str_ = fmt::format("[tcp-client] {}:{}", endpoint.address().to_string(), endpoint.port());
            startRead();
        }
        else
        {
            spdlog::debug("ERROR: {}", ec.message());
            disconnect();
        }
    }
    void startRead()
    {
        socket_.async_receive(asio::buffer(buffer_rx_),
                              std::bind(&Impl::handleRead, this, std::placeholders::_1, std::placeholders::_2));
    }
    void handleRead(const std::error_code &error, std::size_t length)
    {
        if (!error)
        {
            onReceive({buffer_rx_.begin(), buffer_rx_.begin() + length});
            if (should_run_)
                startRead();
        }
        else
        {
            spdlog::error("Error while receiving {}", error.message());
            disconnect();
        }
    }

    void doWrite() override
    {
        if (!should_run_)
            return;
        startWrite();
    }

    void startWrite()
    {
        if (write_in_progress_)
            return;
        std::unique_lock<std::mutex> l{write_lock_};
        if (tx_current_.size() == 0 && tx_queue_.is_not_empty())
            tx_current_ = tx_queue_.pop_back();
        else
            return;
        write_in_progress_ = true;
        socket_.async_send(asio::buffer(tx_current_), [this](asio::error_code ec, std::size_t written) {
            if (!ec)
            {
                if (tx_current_.size() > written)
                    tx_current_.erase(tx_current_.begin(), tx_current_.begin() + written);
                else if (tx_queue_.is_not_empty())
                    tx_current_ = std::forward<std::vector<uint8_t>>(tx_queue_.pop_back());
                else
                    tx_current_.clear();
                write_in_progress_ = false;
                startWrite();
            }
        });
    }

  public:
    Options opts_;
    std::string connection_str_;

  private:
    std::mutex write_lock_;
    std::atomic_bool write_in_progress_;
    tcp::resolver resolver_;
    std::array<uint8_t, 65535> buffer_rx_;
    tcp::socket socket_;
    tcp::resolver::results_type endpoints_;
    bool should_run_;
};

TcpClient::TcpClient(Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

TcpClient::~TcpClient()
{}

void TcpClient::setOptions(const Options &opts)
{
    impl_->setOptions(opts);
    if (impl_->isConnected())
    {
        impl_->disconnect();
        impl_->connect();
    }
}

const TcpClient::Options &TcpClient::options() const
{
    return impl_->opts_;
}

void TcpClient::send(std::span<uint8_t> data)
{
    impl_->write(data);
}

std::string TcpClient::generateReadableName() const
{
    return impl_->connection_str_;
}

bool TcpClient::isConnected() const
{
    return impl_->isConnected();
}
void TcpClient::connect()
{
    impl_->connect();
}
void TcpClient::disconnect()
{
    impl_->disconnect();
}
sig::connection TcpClient::connectOnReceive(const RxSig::slot_type &sub)
{
    return impl_->connectOnReceive(sub);
}
} // namespace dev::con
