#pragma once
#include <memory>
#include <span>
#include <string>
#include "connection.hpp"
#include "manager.hpp"
#include "types.hpp"
namespace dev::con
{

class LIBCON_EXPORT CoapClient : public BaseConnection
{
  public:
    //! https://datatracker.ietf.org/doc/html/rfc7252#section-12.8
    //! IPv4 Addr: 224.0.1.x
    //! IPv6 Addr: FF0X::FD
    static void DiscoverServers(Manager &manager,
                                const std::string_view multicast_ip = "ff02::fd",
                                const uint16_t port = 5683);
    static int DefaultPort();
    static int DefaultSecurePort();

  public:
    struct Options
    {
        std::string server_uri;
        unsigned short server_port;
    };

    enum class Method
    {
        m_get,
        m_post,
        m_put,
        m_delete
    };
    using RequestId = uint32_t;
    static constexpr RequestId kRequestIdWellKnown = std::numeric_limits<RequestId>::max();
    static constexpr RequestId kRequestIdUnknown = kRequestIdWellKnown - 1;

    using RxSig = sig::signal<void(RequestId, std::span<const uint8_t>)>;

  public:
    CoapClient(Manager &manager);
    virtual ~CoapClient();

    void setOptions(const Options &opts);
    const Options &options() const;

    void getWellKnown(const std::string path = ".well-known/core");
    void makeRequest(const Method method,
                     const std::string &path,
                     const RequestId id,
                     const std::span<const uint8_t> data = {});
    sig::connection connectOnReceive(const RxSig::slot_type &sub);

    std::string generateReadableName() const override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
