#pragma once
#include <filesystem>
#include <plugin.hpp>
#include <sol/sol.hpp>
namespace dev::plg
{
class LuaPlugin final : public Plugin
{
  public:
    LuaPlugin(const std::filesystem::path &plugin_file);
    ~LuaPlugin();

    const std::string &name() const override;
    const std::string &description() const override;
    std::vector<ConnDescriptor> getConnections(con::Manager& manager) const override;

  private:
    sol::state lua_;

    std::string name_;
    std::string description_;
};
} // namespace dev::plg
