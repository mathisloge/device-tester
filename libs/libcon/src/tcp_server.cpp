#include "libcon/tcp_server.hpp"
#include <queue>
#include <set>
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
    void send(std::span<uint8_t> data) override;
    const std::string &connectionReadableName() const override;

  private:
    void startRead();
    void startWrite();

  private:
    std::atomic_bool write_in_progress_;
    bool do_run_;
    tcp::socket socket_;
    TcpServerManager &manager_;
    std::array<uint8_t, 65535> rx_buffer_;
    std::string connection_name_;
    std::mutex tx_buffer_mtx_;
    std::queue<std::vector<uint8_t>> tx_buffer_;
};
using TcpServerClientPtr = std::shared_ptr<TcpServerClient>;
using TcpServerClients = std::set<TcpServerClientPtr>;

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
        client->stop();
        connections_.erase(client);
    }
    void stopAll()
    {
        for (auto c : connections_)
            c->stop();
        connections_.clear();
    }
    const TcpServerClients &clients() const
    {
        return connections_;
    }

  private:
    TcpServerClients connections_;
};

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
        {
            acceptor_.cancel();
            acceptor_.close();
        }
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
                startAccept();
            }
        });
    }

  public:
    Options opts_;
    std::string readable_name_;
    std::string connection_name_;
    TcpServerManager manager_;

  private:
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

void TcpServer::sendToAll(std::span<uint8_t> data)
{
    for (const auto &c : impl_->manager_.clients())
    {
        c->send(data);
    }
}

int TcpServer::numberOfConnectedClients() const
{
    return impl_->manager_.clients().size();
}

////! CLIENT
TcpServerClient::TcpServerClient(tcp::socket socket, TcpServerManager &manager)
    : do_run_{true}
    , socket_{std::move(socket)}
    , manager_{manager}
{}
void TcpServerClient::start()
{
    do_run_ = true;
    startRead();
    std::string t{"hallo"};
    std::span<uint8_t> x((uint8_t *)t.data(), t.size());
    send(x);
}
void TcpServerClient::stop()
{
    do_run_ = false;
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
    socket_.async_receive(asio::buffer(rx_buffer_), [self](asio::error_code ec, std::size_t bytes_transferred) {
        if (!ec)
        {
            /*handler_.processData(
                shared_from_this(),
                std::span<uint8_t>(rx_buffer_.begin(), rx_buffer_.begin() +
               bytes_transferred));*/
            if (self->do_run_)
                self->startRead();
        }
        else if (ec != asio::error::operation_aborted)
        {
            spdlog::error("TcpServerClient: {}", ec.message());
            if (self->do_run_)
                self->manager_.stop(self);
        }
    });
}

void TcpServerClient::send(std::span<uint8_t> data)
{
    {
        std::unique_lock<std::mutex> l{tx_buffer_mtx_};
        tx_buffer_.emplace(std::vector<uint8_t>{data.begin(), data.end()});
    }
    startWrite();
}

void TcpServerClient::startWrite()
{
    if (write_in_progress_ || tx_buffer_.size() == 0)
        return;
    write_in_progress_ = true;
    auto self(shared_from_this());
    socket_.async_send(asio::buffer(tx_buffer_.front()), [self](asio::error_code ec, std::size_t written) {
        if (!ec)
        {
            {
                std::unique_lock<std::mutex> l{self->tx_buffer_mtx_};
                self->tx_buffer_.pop();
                self->write_in_progress_ = false;
            }
            self->startWrite();
        }
    });
}

const std::string &TcpServerClient::connectionReadableName() const
{
    return connection_name_;
}
} // namespace dev::con
