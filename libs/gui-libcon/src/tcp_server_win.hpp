#pragma once
#include <memory>
#include <libcon/tcp_server.hpp>
#include "gui-libcon/connection_win.hpp"
#include <gui-common/window.hpp>

namespace dev::gui
{
class TcpServerWin final : public ConnectionWin
{
  public:
    explicit TcpServerWin(con::Manager &manager);
    ~TcpServerWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;

  private:
    std::shared_ptr<con::TcpServer> server_;
    con::TcpServer::Options opts_;
};
} // namespace dev::gui
