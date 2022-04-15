#pragma once
#include <filesystem>
#include <lua_base/lua_base.hpp>
#include <sol/sol.hpp>
namespace dev::lua
{
class LuaPlugin final : public LuaPluginBase
{
  public:
    LuaPlugin(const std::filesystem::path &plugin_file);
    ~LuaPlugin();

    const std::string &name() const override;
    const std::string &description() const override;

  private:
    sol::state lua_;

    std::string name_;
    std::string description_;
};
} // namespace dev::lua
