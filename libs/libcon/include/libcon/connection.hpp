#pragma once
#include <functional>
#include <memory>
#include <span>
#include <string_view>
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

    virtual const std::string &connectionReadableName() const = 0;
    const std::string &readableName() const;
    void setReadableName(std::string_view name);

  private:
    class BaseImpl;
    std::unique_ptr<BaseImpl> base_impl_;
};

using onData = std::function<void(std::span<uint8_t>)>;
} // namespace dev::con
