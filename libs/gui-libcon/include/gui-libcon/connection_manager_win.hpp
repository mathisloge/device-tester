#pragma once
#include <memory>
#include <unordered_set>
#include <libcon/coap_client.hpp>
#include <libcon/manager.hpp>
#include <libcon/serial.hpp>
#include <libcon/tcp_client.hpp>
#include <libcon/tcp_server.hpp>
#include <libcon/udp_connection.hpp>
#include "connection_win.hpp"
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>
namespace dev::gui
{
class ConnectionManagerWin final : public Window
{
  public:
    explicit ConnectionManagerWin(WindowManager &win_manager, const std::shared_ptr<con::Manager> &manager);
    ~ConnectionManagerWin();
    void registerConnection(const std::shared_ptr<con::CoapClient> &con);
    void registerConnection(const std::shared_ptr<con::Serial> &con);
    void registerConnection(const std::shared_ptr<con::TcpClient> &con);
    void registerConnection(const std::shared_ptr<con::TcpServer> &con);
    void registerConnection(const std::shared_ptr<con::UdpConnection> &con);

  private:
    void updateContent() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::Manager> manager_;
    std::unordered_set<std::shared_ptr<ConnectionWin>> connections_;
    std::shared_ptr<ConnectionWin> current_;
};
} // namespace dev::gui
