#pragma once
#include <filesystem>
#include <memory>
#include "lua_base_export.hpp"
namespace dev::lua
{

class LUA_BASE_EXPORT LuaPluginBase
{
  public:
    LuaPluginBase();
    virtual const std::string &name() const = 0;
    virtual const std::string &description() const = 0;
    virtual ~LuaPluginBase();
};

class LUA_BASE_EXPORT LuaBase final
{
  public:
    LuaBase();
    ~LuaBase();
    LuaPluginBase &addPlugin(const std::filesystem::path &plugin_file);

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace dev::lua
