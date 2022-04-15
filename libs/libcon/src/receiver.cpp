#include "receiver.hpp"
namespace dev::con
{
Receiver::Receiver()
{}
Receiver::~Receiver()
{}

void Receiver::onReceive(std::span<uint8_t> data)
{
    resv_sig_(data);
}
sig::connection Receiver::connectOnReceive(const Connection::RxSig::slot_type &sub)
{
    return resv_sig_.connect(sub);
}
}
