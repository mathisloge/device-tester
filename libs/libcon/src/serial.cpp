#include "libcon/serial.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "basic_client.hpp"
#include "manager_impl.hpp"

namespace dev::con
{

class Serial::Impl final : public BasicClient
{
  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl().ctx()}
        , serial_{strand_}
        , should_run_{false}
        , is_open_{false}
    {
        opts_.port = "COM3";
        opts_.character_size = 8;
        opts_.baud_rate = 115200;
    }

    void open()
    {
        asio::post(strand_, std::bind(&Impl::doOpen, this));
    }

    void close()
    {
        should_run_ = false;
        asio::post(strand_, std::bind(&Impl::doClose, this));
    }

    bool isOpen() const
    {
        return serial_.is_open();
    }

  private:
    void doOpen()
    {
        doClose();
        serial_.open(opts_.port);
        serial_.set_option(asio::serial_port_base::baud_rate(opts_.baud_rate));
        serial_.set_option(asio::serial_port_base::character_size(opts_.character_size));
        serial_.set_option(
            asio::serial_port_base::parity([](const Serial::Parity p) -> asio::serial_port_base::parity::type {
                using PT = asio::serial_port_base::parity::type;
                switch (p)
                {
                case Parity::none:
                    return PT::none;
                case Parity::odd:
                    return PT::even;
                case Parity::even:
                    return PT::even;
                default:
                    return PT::none;
                }
            }(opts_.parity)));
        serial_.set_option(
            asio::serial_port_base::stop_bits([](const Serial::StopBits p) -> asio::serial_port_base::stop_bits::type {
                using PT = asio::serial_port_base::stop_bits::type;
                switch (p)
                {
                case StopBits::one:
                    return PT::one;
                case StopBits::onepointfive:
                    return PT::onepointfive;
                case StopBits::two:
                    return PT::two;
                default:
                    return PT::one;
                }
            }(opts_.stop_bits)));
        serial_.set_option(asio::serial_port_base::flow_control(
            [](const Serial::FlowControl p) -> asio::serial_port_base::flow_control::type {
                using PT = asio::serial_port_base::flow_control::type;
                switch (p)
                {
                case FlowControl::none:
                    return PT::none;
                case FlowControl::software:
                    return PT::software;
                case FlowControl::hardware:
                    return PT::hardware;
                default:
                    return PT::none;
                }
            }(opts_.flow_control)));
        should_run_ = true;
        is_open_ = true;
        connection_str_ = fmt::format("serial://{}@{}", opts_.port, opts_.baud_rate);

        asio::post(strand_, std::bind(&Impl::doRead, this));
    }
    void doClose()
    {
        is_open_ = false;
        should_run_ = false;
        if (serial_.is_open())
        {
            asio::error_code ec;
            serial_.cancel(ec);
            serial_.close(ec);
        }
    }
    void doWrite() override
    {
        if (!should_run_)
            return;
        asio::post(strand_, std::bind(&Impl::startWrite, this));
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
        serial_.async_write_some(asio::buffer(tx_current_), [this](asio::error_code ec, std::size_t written) {
            if (!ec)
            {
                if (tx_current_.size() > written)
                    tx_current_.erase(tx_current_.begin(), tx_current_.begin() + written);
                else if (tx_queue_.is_not_empty())
                    tx_current_ = std::forward<std::vector<uint8_t>>(tx_queue_.pop_back());
                else
                    tx_current_.clear();
                write_in_progress_ = false;
                asio::post(strand_, std::bind(&Impl::startWrite, this));
            }
        });
    }

    void doRead()
    {
        serial_.async_read_some(asio::buffer(buffer_rx_),
                                std::bind(&Impl::handleRead, this, std::placeholders::_1, std::placeholders::_2));
    }
    void handleRead(const asio::error_code &error, std::size_t n)
    {
        if (!error)
        {
            onReceive(std::span<uint8_t>(buffer_rx_.begin(), buffer_rx_.begin() + n));
            if (should_run_)
                asio::post(strand_, std::bind(&Impl::doRead, this));
        }
        else
        {
            SPDLOG_ERROR("Error while receiving {}", error.message());
            doClose();
        }
    }

  public:
    Options opts_;
    asio::serial_port serial_;
    std::string connection_str_;

    bool should_run_;
    bool is_open_;
    std::array<uint8_t, 65535> buffer_rx_;
    std::mutex write_lock_;
    std::atomic_bool write_in_progress_;
};

Serial::Serial(Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

Serial::~Serial()
{}

void Serial::send(std::span<uint8_t> data)
{
    impl_->write(data);
}

const Serial::Options &Serial::options() const
{
    return impl_->opts_;
}
void Serial::setOptions(const Options &opts)
{
    impl_->opts_ = opts;
}

std::string Serial::generateReadableName() const
{
    return impl_->connection_str_;
}

sig::connection Serial::connectOnReceive(const RxSig::slot_type &sub)
{
    return impl_->connectOnReceive(sub);
}

bool Serial::isOpen() const
{
    return impl_->isOpen();
}
void Serial::open()
{
    impl_->open();
}
void Serial::close()
{
    impl_->close();
}

} // namespace dev::con
