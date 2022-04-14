#pragma once
#include <memory>
#include <libcon/udp_connection.hpp>
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>
#include "raw_data_win.hpp"
#include "gui-libcon/connection_win.hpp"

namespace dev::gui
{
class UdpConWin final : public ConnectionWin
{
  public:
    explicit UdpConWin(con::Manager &manager, WindowManager &win_manager);
    ~UdpConWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::UdpConnection> connection_;
    std::shared_ptr<RawDataWin> raw_win_;
    con::UdpConnection::Options opts_;
};
} // namespace dev::gui