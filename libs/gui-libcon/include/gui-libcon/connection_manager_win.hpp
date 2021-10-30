#pragma once
#include <memory>
#include <vector>
#include <libcon/manager.hpp>
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>

#include "connection_win.hpp"
namespace dev::gui
{
class ConnectionManagerWin final : public Window
{
  public:
    explicit ConnectionManagerWin(WindowManager &win_manager, const std::shared_ptr<con::Manager> &manager);
    ~ConnectionManagerWin();

  private:
    void updateContent() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::Manager> manager_;
    std::vector<std::shared_ptr<ConnectionWin>> connections_;
    std::shared_ptr<ConnectionWin> current_;
};
} // namespace dev::gui
