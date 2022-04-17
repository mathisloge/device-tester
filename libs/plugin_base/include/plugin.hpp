#pragma once
#include <libcon/connection.hpp>
#include <libcon/manager.hpp>
namespace dev::plg
{
enum class ConnectionType
{
    ct_coap_client,
    ct_tcp_client,
    ct_tcp_server,
    ct_udp_connection,
    ct_serial_connection,
    ct_unknown
};
class Plugin
{
  public:
    Plugin() = default;
    ~Plugin() = default;

    virtual const std::string &name() const = 0;
    virtual const std::string &description() const = 0;
    using ConnDescriptor = std::pair<ConnectionType, std::shared_ptr<con::BaseConnection>>;
    [[nodiscard]] virtual std::vector<ConnDescriptor> getConnections(con::Manager &manager) const = 0;
};
} // namespace dev::plg
