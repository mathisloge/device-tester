#pragma once
#include <functional>
#include <memory>
#include <span>
#include <string_view>
#include <boost/signals2.hpp>
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
    using ReiceiveSignal = boost::signals2::signal<void(std::span<uint8_t>)>;

  public:
    Connection();
    virtual ~Connection();
    Connection(const Connection &) = delete;
    Connection(Connection &&) = delete;
    Connection &operator=(const Connection &) = delete;
    Connection &operator=(Connection &&) = delete;

    virtual boost::signals2::connection connectOnReceive(const ReiceiveSignal::slot_type &sub) = 0;
    virtual void send(std::span<uint8_t> data) = 0;

    virtual const std::string &connectionReadableName() const = 0;
    const std::string &readableName() const;
    void setReadableName(std::string_view name);

  private:
    class BaseImpl;
    std::unique_ptr<BaseImpl> base_impl_;
};
} // namespace dev::con
