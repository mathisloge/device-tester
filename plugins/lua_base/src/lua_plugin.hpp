#pragma once
#include <filesystem>
#include <sol/sol.hpp>
namespace dev::lua
{
class LuaPlugin final
{
  public:
    LuaPlugin(const std::filesystem::path &plugin_file);
    ~LuaPlugin();

  private:
    sol::state lua_;
};
} // namespace dev::lua
