#pragma once
#include <filesystem>
#include <memory>
#include <libcon/manager.hpp>
#include <plugin.hpp>
#include "libplg_export.hpp"

namespace dev::plg
{
class LIBPLG_EXPORT PluginManager final
{
  public:
    PluginManager(con::Manager &manager);
    ~PluginManager();

    Plugin &loadLuaPlugin(const std::filesystem::path &path_to_plugin_file);
    const std::vector<std::shared_ptr<Plugin>> &plugins() const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::plg
