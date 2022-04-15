#include "lua_plugin.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include "pb.h"
namespace dev::lua
{
LuaPlugin::LuaPlugin(const std::filesystem::path &plugin_file)
{
    lua_.open_libraries(sol::lib::base,
                        sol::lib::coroutine,
                        sol::lib::string,
                        sol::lib::io,
                        sol::lib::table,
                        sol::lib::math,
                        sol::lib::package);
    const std::string package_path = lua_["package"]["path"];
    lua_["package"]["path"] =
        package_path + ";" + std::filesystem::path{"./lua/modules/"}.make_preferred().string() + "?.lua";
    auto res = lua_.script_file(plugin_file.generic_string());
    if (!res.valid())
    {
        sol::error error = res;
        spdlog::error("{}", error.what());
    }

    sol::table plugin = lua_["plugin"];
    sol::table data_generators = plugin["data_generators"];
    name_ = plugin.get<std::string>("name");
    description_ = plugin.get<std::string>("description");
    spdlog::info("Plugin: {}", name_);
    spdlog::info("Description: {}", description_);

    const std::string bytes = data_generators["first_message"]();
    spdlog::info("Bytes: {}", bytes.size());
}
LuaPlugin::~LuaPlugin()
{}

const std::string &LuaPlugin::name() const
{
    return name_;
}
const std::string &LuaPlugin::description() const
{
    return description_;
}
} // namespace dev::lua
