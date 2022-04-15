#pragma once
#include <cstdint>
#include <span>
#include <system_error>
#include "libcon/connection.hpp"

namespace dev::con
{
class Receiver
{
  public:
    Receiver();
    virtual ~Receiver();
    sig::connection connectOnReceive(const Connection::RxSig::slot_type &sub);

  protected:
    void onReceive(std::span<uint8_t> data);

  private:
    Connection::RxSig resv_sig_;
};
} // namespace dev::con
