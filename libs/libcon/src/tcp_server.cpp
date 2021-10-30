#include "libcon/tcp_server.hpp"
#include <unordered_set>
#include <spdlog/spdlog.h>
#include "manager_impl.hpp"
namespace dev::con
{
class TcpServerManager;
class TcpServerClient : public ITcpServerClient, public std::enable_shared_from_this<TcpServerClient>
{
    using tcp = asio::ip::tcp;

  public:
    explicit TcpServerClient(tcp::socket socket, TcpServerManager &manager);
    void start();
    void stop();
    void shutdown();

  private:
    void startRead();

  private:
    tcp::socket socket_;
    TcpServerManager &manager_;
    std::array<uint8_t, 65535> rx_buffer_;
};
using TcpServerClientPtr = std::shared_ptr<TcpServerClient>;

class TcpServerManager final
{
  public:
    TcpServerManager()
    {}
    ~TcpServerManager()
    {
        stopAll();
    }
    void start(TcpServerClientPtr client)
    {
        connections_.insert(client);
        client->start();
    }
    void stop(TcpServerClientPtr client)
    {
        connections_.erase(client);
        client->stop();
    }
    void stopAll()
    {
        for (auto c : connections_)
            c->stop();
        connections_.clear();
    }

  private:
    std::unordered_set<TcpServerClientPtr> connections_;
};

TcpServerClient::TcpServerClient(tcp::socket socket, TcpServerManager &manager)
    : socket_{std::move(socket)}
    , manager_{manager}
{}
void TcpServerClient::start()
{
    startRead();
}
void TcpServerClient::stop()
{
    socket_.close();
}
void TcpServerClient::shutdown()
{
    asio::error_code ignored_ec;
    socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
}

void TcpServerClient::startRead()
{
    auto self{shared_from_this()};
    socket_.async_read_some(asio::buffer(rx_buffer_), [this, self](asio::error_code ec, std::size_t bytes_transferred) {
        if (!ec)
        {
            /*handler_.processData(
                shared_from_this(),
                std::span<uint8_t>(rx_buffer_.begin(), rx_buffer_.begin() +
               bytes_transferred));*/
            startRead();
        }
        else if (ec != asio::error::operation_aborted)
        {
            manager_.stop(shared_from_this());
        }
    });
}

class TcpServer::Impl final
{
    using tcp = asio::ip::tcp;

  public:
    Impl(Manager &manager)
        : strand_{asio::make_strand(manager.impl().ctx())}
        , acceptor_{strand_}
    {}

    ~Impl()
    {
        stop();
    }

    bool isRunning() const
    {
        return acceptor_.is_open();
    }
    void start()
    {

        stop();
        if (opts_.protocol == IpProtocol::none)
        {
            connection_name_ = "[tcp-server] none";
            return;
        }
        connection_name_ =
            fmt::format("[tcp-server] {}::{}", opts_.protocol == IpProtocol::ipv4 ? "ipv4" : "ipv6", opts_.port);
        tcp::endpoint endpoint(opts_.protocol == IpProtocol::ipv4 ? tcp::v4() : tcp::v6(), opts_.port);
        acceptor_.open(endpoint.protocol());
        // acceptor_.set_option(...);
        asio::error_code ec;
        acceptor_.bind(endpoint, ec);
        if (!ec)
        {
            acceptor_.listen();
            startAccept();
        }
        else
        {
            spdlog::error("server err: {}", ec.message());
        }
    }
    void stop()
    {
        if (acceptor_.is_open())
            acceptor_.close();
        manager_.stopAll();
    }
    void setOptions(const Options &opts)
    {
        opts_ = opts;
        if (isRunning())
        {
            start();
        }
    }

  private:
    void startAccept()
    {
        acceptor_.async_accept([this](asio::error_code ec, tcp::socket socket) {
            if (!acceptor_.is_open())
            {
                return;
            }

            if (!ec)
            {
                manager_.start(std::make_shared<TcpServerClient>(std::move(socket), manager_));
            }
            startAccept();
        });
    }

  public:
    Options opts_;
    std::string readable_name_;
    std::string connection_name_;

  private:
    TcpServerManager manager_;
    asio::strand<asio::any_io_executor> strand_;
    tcp::acceptor acceptor_;
};

TcpServer::TcpServer(Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

TcpServer::~TcpServer()
{}

bool TcpServer::isRunning() const
{
    return impl_->isRunning();
}

void TcpServer::start()
{
    impl_->start();
}

void TcpServer::stop()
{
    impl_->stop();
}

void TcpServer::setOptions(const Options &opts)
{
    impl_->setOptions(opts);
}

const TcpServer::Options &TcpServer::options() const
{
    return impl_->opts_;
}

const std::string &TcpServer::connectionReadableName() const
{
    return impl_->connection_name_;
}

const std::string &TcpServer::readableName() const
{
    return impl_->readable_name_;
}

void TcpServer::setReadableName(std::string_view name)
{
    impl_->readable_name_ = name;
}
} // namespace dev::con
