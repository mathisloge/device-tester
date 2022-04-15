#pragma once
#include <lua_base/lua_base.hpp>
#include <gui-common/window.hpp>
namespace dev::gui
{
class PluginTest final : public Window
{
  public:
    PluginTest();
    ~PluginTest();
    void updateContent() override;

  private:
    lua::LuaBase plugin_;
};
} // namespace dev::gui
