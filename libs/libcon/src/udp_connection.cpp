#include "libcon/udp_connection.hpp"
#include <fmt/compile.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "basic_client.hpp"
#include "manager_impl.hpp"
namespace dev::con
{
class UdpConnection::Impl final : public BasicClient
{
    using udp = asio::ip::udp;

  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl().ctx()}
        , socket_{strand_}
        , should_run_{true}
        , should_receive_{false}
        , write_in_progress_{false}
    {}

    ~Impl()
    {
        should_run_ = false;
        disconnect();
    }

    void setOptions(const Options &opts)
    {
        opts_ = opts;
        if (isReceiving())
        {
            disconnect();
            startReceive();
        }
        try
        {
            const auto addr = asio::ip::make_address(opts_.write_address);
            write_endpoint_ = udp::endpoint(asio::ip::make_address(opts_.write_address), opts.write_port);
        }
        catch (const std::exception &e)
        {
            spdlog::error("UdpConnection: error make_addr: {}", e.what());
        }
    }
    void startReceive()
    {
        if (opts_.listen_protocol != IpProtocol::none)
        {
            should_receive_ = true;
            listen_endpoint_ =
                udp::endpoint(opts_.listen_protocol == IpProtocol::ipv4 ? udp::v4() : udp::v6(), opts_.listen_port);
            try
            {
                socket_.open(listen_endpoint_.protocol());
                socket_.bind(listen_endpoint_);
                startRead();
            }
            catch (const std::runtime_error &err)
            {
                should_receive_ = false;
                spdlog::error("Error while binding listen port {}", err.what());
            }
        }
    }
    void disconnect()
    {
        should_receive_ = false;
        asio::error_code e;
        socket_.cancel(e);
    }
    bool isReceiving() const
    {
        return should_receive_;
    }

  private:
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
        socket_.async_send_to(
            asio::buffer(tx_current_), write_endpoint_, [this](asio::error_code ec, std::size_t written) {
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

    void startRead()
    {
        socket_.async_receive(asio::buffer(buffer_rx_),
                              std::bind(&Impl::handleRead, this, std::placeholders::_1, std::placeholders::_2));
    }

    void handleRead(const asio::error_code &error, std::size_t n)
    {
        if (!error)
        {
            onReceive(std::span<uint8_t>(buffer_rx_.begin(), buffer_rx_.begin() + n));
            if (should_receive_)
                startRead();
        }
        else
        {
            SPDLOG_ERROR("Error while receiving {}", error.message());
            disconnect();
        }
    }

  public:
    Options opts_;

  private:
    udp::socket socket_;
    std::array<uint8_t, 65535> buffer_rx_;
    udp::endpoint listen_endpoint_;
    udp::endpoint write_endpoint_;
    bool should_run_;
    bool should_receive_;
    std::mutex write_lock_;
    std::atomic_bool write_in_progress_;
};

UdpConnection::UdpConnection(Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

UdpConnection::~UdpConnection()
{}

sig::connection UdpConnection::connectOnReceive(const RxSig::slot_type &sub)
{
    return impl_->connectOnReceive(sub);
}

void UdpConnection::setOptions(const Options &opts)
{
    impl_->setOptions(opts);
}

void UdpConnection::send(std::span<uint8_t> data)
{
    impl_->write(data);
}

bool UdpConnection::isReceiving() const
{
    return impl_->isReceiving();
}

void UdpConnection::setReceiving(bool is_receiving)
{
    if (is_receiving)
        impl_->startReceive();
    else
        impl_->disconnect();
}

std::string UdpConnection::generateReadableName() const
{
    return fmt::format(FMT_COMPILE("[UDP] {}"), impl_->opts_.write_address);
}

const UdpConnection::Options &UdpConnection::options() const
{
    return impl_->opts_;
}
} // namespace dev::con
