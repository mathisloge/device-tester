#pragma once
#include <memory>
#include <libcon/tcp_client.hpp>
#include "gui-libcon/connection_win.hpp"
#include <gui-common/window.hpp>

namespace dev::gui
{
class TcpClientWin final : public ConnectionWin
{
  public:
    explicit TcpClientWin(con::Manager &manager);
    ~TcpClientWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;
    void onData(std::span<uint8_t> data);

  private:
    std::shared_ptr<con::TcpClient> client_;
    con::TcpClient::Options opts_;
};
} // namespace dev::gui
