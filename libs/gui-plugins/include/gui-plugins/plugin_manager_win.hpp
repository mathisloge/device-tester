#pragma once
#include <lua_base/lua_base.hpp>
#include <gui-common/window.hpp>
namespace dev::gui
{
class PluginManagerWin final : public Window
{
  public:
    PluginManagerWin();
    ~PluginManagerWin();
    void updateContent() override;

  private:
    lua::LuaBase plugin_;
};
} // namespace dev::gui
