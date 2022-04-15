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

    void addPlugin(const std::filesystem::path &plugin_file)
    {
        loaded_plugins_.emplace_back(std::make_unique<LuaPlugin>(plugin_file));
    }

  private:
    std::vector<std::unique_ptr<LuaPlugin>> loaded_plugins_;
};

LuaBase::LuaBase()
    : impl_{std::make_unique<Impl>()}
{}
LuaBase::~LuaBase()
{}
void LuaBase::addPlugin(const std::filesystem::path &plugin_file)
{
    impl_->addPlugin(plugin_file);
}
} // namespace dev::lua
