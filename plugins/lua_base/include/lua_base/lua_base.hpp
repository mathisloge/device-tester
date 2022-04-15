#pragma once
#include <filesystem>
#include <memory>
#include "lua_base_export.hpp"
namespace dev::lua
{
class LUA_BASE_EXPORT LuaBase final
{
  public:
    LuaBase();
    ~LuaBase();
    void addPlugin(const std::filesystem::path &plugin_file);

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::lua
