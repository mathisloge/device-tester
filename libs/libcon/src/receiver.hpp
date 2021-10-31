#pragma once
#include <cstdint>
#include <span>
#include <system_error>
#include "libcon/connection.hpp"
class Receiver
{
  public:
    Receiver();
    virtual ~Receiver();
    boost::signals2::connection connectOnReceive(const dev::con::Connection::ReiceiveSignal::slot_type &sub);

  protected:
    void onReceive(std::span<uint8_t> data);

  private:
    dev::con::Connection::ReiceiveSignal resv_sig_;
};
