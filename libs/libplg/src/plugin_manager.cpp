#include <libplg/plugin_manager.hpp>
#include <libplglua/lua_plugin.hpp>
#include <plugin.hpp>
namespace dev::plg
{

class PluginManager::Impl final
{
  public:
    Impl(con::Manager &manager)
        : manager_{manager}
    {}
    ~Impl()
    {}

    Plugin &loadLuaPlugin(const std::filesystem::path &file)
    {
        return *loaded_plugins_.emplace_back(std::make_unique<LuaPlugin>(manager_, file));
    }

  private:
  private:
    con::Manager &manager_;
    std::vector<std::unique_ptr<Plugin>> loaded_plugins_;
};

PluginManager::PluginManager(con::Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

PluginManager::~PluginManager()
{}

Plugin &PluginManager::loadLuaPlugin(const std::filesystem::path &path_to_plugin_file)
{
    return impl_->loadLuaPlugin(path_to_plugin_file);
}
} // namespace dev::plg
