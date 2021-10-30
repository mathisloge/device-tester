#include "libcon/tcp_client.hpp"
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
    Impl(Manager &manager, const onData &read_cb)
        : BasicClient{manager.impl().ctx()}
        , opts_{}
        , socket_{strand_}
        , resolver_{strand_}
        , read_cb_{read_cb}
    {}

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
        spdlog::debug("Connecting to: {}:{}", opts_.server, opts_.server_port);
        resolver_.async_resolve(
            opts_.server,
            fmt::format("{}", opts_.server_port),
            [this](const asio::error_code &ec, tcp::resolver::results_type endpoints) {
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
        socket_.close();
    }

  private:
    void handleConnect(const std::error_code &ec, const tcp::endpoint &endpoint)
    {
        if (!ec)
        {
            spdlog::debug("[tcp] {}:{}", endpoint.address().to_string(), endpoint.port());
            connection_str_ = fmt::format("[tcp] {}:{}", endpoint.address().to_string(), endpoint.port());
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
            // handle_.processData(std::span<uint8_t>(buffer_rx_.begin(), buffer_rx_.end()));
            // buffer_rx_.clear();
            // if (should_run_)
            read_cb_({buffer_rx_.begin(), buffer_rx_.begin() + length});
            startRead();
        }
        else
        {
            spdlog::error("Error while receiving {}", error.message());
            disconnect();
        }
    }

  public:
    Options opts_;
    std::string connection_str_;

  private:
    tcp::resolver resolver_;
    onData read_cb_;
    std::array<uint8_t, 65535> buffer_rx_;
    tcp::socket socket_;
    tcp::resolver::results_type endpoints_;
};

TcpClient::TcpClient(Manager &manager, const onData &read_cb)
    : impl_{std::make_unique<Impl>(manager, read_cb)}
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
{}

const std::string &TcpClient::connectionReadableName() const
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
} // namespace dev::con
