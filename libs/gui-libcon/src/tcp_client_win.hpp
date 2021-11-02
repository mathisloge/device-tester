#pragma once
#include <memory>
#include <libcon/tcp_client.hpp>
#include "raw_data_win.hpp"
#include "gui-libcon/connection_win.hpp"
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>
namespace dev::gui
{
class TcpClientWin final : public ConnectionWin
{
  public:
    explicit TcpClientWin(con::Manager &manager, WindowManager &win_manager);
    ~TcpClientWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::TcpClient> client_;
    std::shared_ptr<RawDataWin> raw_win_;
    con::TcpClient::Options opts_;
};
} // namespace dev::gui
