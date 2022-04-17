#pragma once
#include <memory>
#include <libcon/manager.hpp>
#include <libplg/plugin_manager.hpp>
#include <gui-common/window.hpp>
namespace dev::gui
{
class PluginManagerWin final : public Window
{
  public:
    explicit PluginManagerWin(std::shared_ptr<dev::con::Manager> connection_manager);
    ~PluginManagerWin();
    void updateContent() override;

  private:
    plg::PluginManager manager_;
    std::shared_ptr<dev::con::Manager> connection_manager_;
};
} // namespace dev::gui
