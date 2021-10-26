#pragma once
#include <memory>
#include <span>
#include "libcon_export.hpp"

namespace dev::con
{
enum class IpProtocol
{
    none,
    ipv4,
    ipv6
};
class LIBCON_EXPORT Connection
{
  public:
    Connection();
    virtual ~Connection();
    Connection(const Connection &) = delete;
    Connection(Connection &&) = delete;
    Connection &operator=(const Connection &) = delete;
    Connection &operator=(Connection &&) = delete;

    virtual void send(std::span<uint8_t> data) = 0;
};
} // namespace dev::con
