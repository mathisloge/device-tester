#pragma once
#include <filesystem>
#include <plugin.hpp>
#include <sol/sol.hpp>
namespace dev::plg
{
class LuaPlugin final : public Plugin
{
  public:
    LuaPlugin(con::Manager &manager, const std::filesystem::path &plugin_file);
    ~LuaPlugin();

    const std::string &name() const override;
    const std::string &description() const override;
    const std::vector<ConnDescriptor> &getConnections() const override;
    const std::vector<std::unique_ptr<ActionItem>> &getActionItems() const override;

  private:
    void setupConnections(con::Manager &manager);
    void setupActions();

  private:
    sol::state lua_;

    std::string name_;
    std::string description_;
    std::vector<ConnDescriptor> connections_;
    std::vector<std::unique_ptr<ActionItem>> actions_;
};
} // namespace dev::plg
