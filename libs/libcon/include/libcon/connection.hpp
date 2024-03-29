#pragma once
#include <functional>
#include <memory>
#include <span>
#include <string_view>
#include "libcon_export.hpp"
#include "types.hpp"

namespace dev::con
{
enum class IpProtocol
{
    none,
    ipv4,
    ipv6
};

class LIBCON_EXPORT BaseConnection
{
  public:
    BaseConnection();
    virtual ~BaseConnection();
    BaseConnection(const BaseConnection &) = delete;
    BaseConnection(BaseConnection &&) = delete;
    BaseConnection &operator=(const BaseConnection &) = delete;
    BaseConnection &operator=(BaseConnection &&) = delete;

    const std::string &readableName() const;
    void setReadableName(const std::string_view name);
    virtual std::string generateReadableName() const = 0;

  private:
    class BaseImpl;
    std::unique_ptr<BaseImpl> base_impl_;
};

class LIBCON_EXPORT Connection : public BaseConnection
{
  public:
    using RxSig = sig::signal<void(std::span<uint8_t>)>;

  public:
    Connection();
    virtual ~Connection();
    virtual sig::connection connectOnReceive(const RxSig::slot_type &sub) = 0;
    virtual void send(std::span<uint8_t> data) = 0;
};
} // namespace dev::con
