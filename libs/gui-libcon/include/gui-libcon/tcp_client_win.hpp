#pragma once
#include <memory>
#include <libcon/tcp_client.hpp>
#include "connection_win.hpp"
#include <gui-common/window.hpp>

namespace dev::gui
{
class TcpClientWin final : public ConnectionWin
{
  public:
    explicit TcpClientWin(const std::shared_ptr<con::TcpClient> &client);
    ~TcpClientWin();

    const std::string &title() const override;
    void drawTabSettings() override;

  private:
    std::shared_ptr<con::TcpClient> client_;
};
} // namespace dev::gui
