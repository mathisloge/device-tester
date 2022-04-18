#pragma once
#include <memory>
#include <libcon/manager.hpp>
#include <libplg/plugin_manager.hpp>
#include <gui-common/window.hpp>
#include <gui-libcon/connection_manager_win.hpp>
namespace dev::gui
{
class PluginManagerWin final : public Window
{
  public:
    explicit PluginManagerWin(std::shared_ptr<con::Manager> connection_manager,
                              ConnectionManagerWin &connection_manager_win);
    ~PluginManagerWin();
    void updateContent() override;

  private:
    plg::PluginManager manager_;
    std::shared_ptr<con::Manager> connection_manager_;
    ConnectionManagerWin &connection_manager_win_;
};
} // namespace dev::gui
