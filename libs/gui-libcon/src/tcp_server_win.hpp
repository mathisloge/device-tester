#pragma once
#include <memory>
#include <unordered_map>
#include <libcon/tcp_server.hpp>
#include "raw_data_win.hpp"
#include "gui-libcon/connection_win.hpp"
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>
namespace dev::gui
{
class TcpServerWin final : public ConnectionWin
{
  public:
    explicit TcpServerWin(con::Manager &manager, WindowManager &win_manager);
    ~TcpServerWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;
    void drawCustomTabs() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::TcpServer> server_;
    con::TcpServer::Options opts_;

    std::unordered_map<con::ITcpServerClient *, std::shared_ptr<RawDataWin>> client_wins_;
};
} // namespace dev::gui
