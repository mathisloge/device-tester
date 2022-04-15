#include <lua_base/lua_base.hpp>
#include <sol/sol.hpp>
#include "lua_plugin.hpp"
namespace dev::lua
{
class LuaBase::Impl final
{
  public:
    Impl()
    {}
    ~Impl()
    {}

    LuaPluginBase &addPlugin(const std::filesystem::path &plugin_file)
    {
        return *loaded_plugins_.emplace_back(std::make_unique<LuaPlugin>(plugin_file));
    }

  private:
    std::vector<std::unique_ptr<LuaPlugin>> loaded_plugins_;
};

LuaBase::LuaBase()
    : impl_{std::make_unique<Impl>()}
{}
LuaBase::~LuaBase()
{}
LuaPluginBase &LuaBase::addPlugin(const std::filesystem::path &plugin_file)
{
    return impl_->addPlugin(plugin_file);
}

LuaPluginBase::LuaPluginBase()
{}
LuaPluginBase::~LuaPluginBase()
{}
} // namespace dev::lua
