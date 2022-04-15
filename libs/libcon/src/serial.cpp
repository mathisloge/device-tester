#include "libcon/serial.hpp"
#include <fmt/format.h>
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
    {}

    void connectPort()
    {
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
                }
            }(opts_.flow_control)));

        connection_str_ = fmt::format("serial://{}@{}", opts_.port, opts_.baud_rate);
    }

  private:
    void doWrite() override
    {}

  public:
    Options opts_;
    asio::serial_port serial_;
    std::string connection_str_;
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

} // namespace dev::con
