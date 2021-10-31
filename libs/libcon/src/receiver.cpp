#include "receiver.hpp"

Receiver::Receiver()
{}
Receiver::~Receiver()
{}

void Receiver::onReceive(std::span<uint8_t> data)
{
    resv_sig_(data);
}
boost::signals2::connection Receiver::connectOnReceive(const dev::con::Connection::ReiceiveSignal::slot_type &sub)
{
    return resv_sig_.connect(sub);
}
