#pragma once
#include <memory>
#include <libcon/serial.hpp>
#include "raw_data_win.hpp"
#include "gui-libcon/connection_win.hpp"
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>

namespace dev::gui
{
class SerialWin final : public ConnectionWin
{
  public:
    SerialWin(con::Manager &manager, WindowManager &win_manager);
    SerialWin(const std::shared_ptr<con::Serial> &connection, WindowManager &win_manager);
    ~SerialWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::Serial> connection_;
    std::shared_ptr<RawDataWin> raw_win_;
    con::Serial::Options opts_;
};
} // namespace dev::gui
