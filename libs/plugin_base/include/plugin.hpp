#pragma once
#include <variant>
#include <libcon/coap_client.hpp>
#include <libcon/manager.hpp>
#include <libcon/serial.hpp>
#include <libcon/tcp_client.hpp>
#include <libcon/tcp_server.hpp>
#include <libcon/udp_connection.hpp>
namespace dev::plg
{
class ActionItem
{
  public:
    ActionItem() = default;
    ~ActionItem() = default;

    virtual void drawGui() = 0;
};
class Plugin
{
  public:
    Plugin() = default;
    ~Plugin() = default;

    virtual const std::string &name() const = 0;
    virtual const std::string &description() const = 0;
    using ConnDescriptor = std::variant<std::shared_ptr<con::CoapClient>,
                                        std::shared_ptr<con::Serial>,
                                        std::shared_ptr<con::TcpClient>,
                                        std::shared_ptr<con::TcpServer>,
                                        std::shared_ptr<con::UdpConnection>>;
    virtual const std::vector<ConnDescriptor> &getConnections() const = 0;
    virtual const std::vector<std::unique_ptr<ActionItem>> &getActionItems() const = 0;
};

} // namespace dev::plg
