#pragma once
#include <memory>
#include <vector>
#include <libcon/manager.hpp>
#include "connection_win.hpp"
#include <gui-common/window.hpp>
namespace dev::gui
{
class ConnectionManagerWin final : public Window
{
  public:
    explicit ConnectionManagerWin(const std::shared_ptr<con::Manager> &manager);
    ~ConnectionManagerWin();

  private:
    void updateContent() override;

  private:
    std::shared_ptr<con::Manager> manager_;
    std::vector<std::shared_ptr<ConnectionWin>> connections_;
    std::shared_ptr<ConnectionWin> current_;
};
} // namespace dev::gui
