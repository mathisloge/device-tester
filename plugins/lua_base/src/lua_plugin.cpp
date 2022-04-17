#include <iostream>
#include <libcon/coap_client.hpp>
#include <libplglua/lua_plugin.hpp>
#include <spdlog/spdlog.h>
namespace dev::plg
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
    name_ = plugin.get<std::string>("name");
    description_ = plugin.get<std::string>("description");
    spdlog::info("Plugin: {}", name_);
    spdlog::info("Description: {}", description_);
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

std::vector<Plugin::ConnDescriptor> LuaPlugin::getConnections(con::Manager &manager) const
{
    std::vector<Plugin::ConnDescriptor> cons;
    const sol::table wanted_cons = lua_["plugin"]["connection"];
    for (const auto &key_value_pair : wanted_cons)
    {
        const sol::table decr_table = key_value_pair.second;
        const auto type_str = decr_table["type"].get<std::string>();
        const auto type = [](const std::string &type_str) -> ConnectionType {
            if (type_str == "coap")
                return ConnectionType::ct_coap_client;
            return ConnectionType::ct_unknown;
        }(type_str);
        const auto name_str = decr_table["name"].get<std::string>();

        std::shared_ptr<con::BaseConnection> connection{nullptr};
        switch (type)
        {
        case ConnectionType::ct_coap_client: {
            auto new_con = std::make_shared<con::CoapClient>(manager);
            new_con->setReadableName(name_str);

            connection = std::move(new_con);
            break;
        }
        }

        if (connection)
            cons.emplace_back(Plugin::ConnDescriptor{type, std::move(connection)});
    }
    return cons;
}
} // namespace dev::plg
