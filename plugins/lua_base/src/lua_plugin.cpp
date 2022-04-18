#include <iostream>
#include <imgui.h>
#include <libcon/coap_client.hpp>
#include <libplglua/lua_plugin.hpp>
#include <spdlog/spdlog.h>
namespace dev::plg
{
struct BaseConnectionWrapper
{};
struct ConnectionWrapper : public BaseConnectionWrapper
{
    ConnectionWrapper(asio::io_context &io, const std::shared_ptr<con::Connection> &client, sol::function cb)
        : strand{asio::make_strand(io)}
        , client{client}
        , callback_fnc{cb}
    {
        client->connectOnReceive(std::bind(&ConnectionWrapper::onReceive, this, std::placeholders::_1));
    }

    void onReceive(std::span<const uint8_t> data)
    {
        if (callback_fnc)
        {
            std::shared_ptr<std::vector<uint8_t>> data_mem = std::make_shared<std::vector<uint8_t>>();
            data_mem->assign(data.begin(), data.end());
            asio::post(strand, [this, data_mem]() { callback_fnc(*data_mem); });
        }
    }

    asio::strand<asio::any_io_executor> strand;
    std::shared_ptr<con::Connection> client;
    sol::function callback_fnc;
};

struct CoapClientConnectionWrapper : public BaseConnectionWrapper
{
    CoapClientConnectionWrapper(asio::io_context &io, const std::shared_ptr<con::CoapClient> &client, sol::function cb)
        : strand{asio::make_strand(io)}
        , client{client}
        , callback_fnc{cb}
    {
        client->connectOnReceive(
            std::bind(&CoapClientConnectionWrapper::onReceive, this, std::placeholders::_1, std::placeholders::_2));
    }

    void onReceive(con::CoapClient::RequestId id, std::span<const uint8_t> data)
    {
        if (callback_fnc)
        {
            std::shared_ptr<std::vector<uint8_t>> data_mem = std::make_shared<std::vector<uint8_t>>();
            data_mem->assign(data.begin(), data.end());
            asio::post(strand, [this, id, data_mem]() { callback_fnc(id, *data_mem); });
        }
    }

    void put(const con::CoapClient::RequestId id, const std::string path, std::vector<uint8_t> data)
    {
        client->makeRequest(con::CoapClient::Method::m_put, path, id, data);
    }
    //! todo: this is stupid... the strand has to be in the lua plugin instance to guard multithreaded execution of the sol::state.
    asio::strand<asio::any_io_executor> strand;
    std::shared_ptr<con::CoapClient> client;
    sol::function callback_fnc;
};

class LuaActionItem : public ActionItem
{
  public:
    LuaActionItem(sol::function draw_fnc)
        : draw_fnc_{draw_fnc}
    {}
    ~LuaActionItem()
    {}
    void drawGui() override
    {
        draw_fnc_();
    }

  private:
    sol::function draw_fnc_;
};

LuaPlugin::LuaPlugin(con::Manager &manager, const std::filesystem::path &plugin_file)
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

    // clang-format off
    auto coap_client_type =
        lua_.new_usertype<CoapClientConnectionWrapper>("coap_client", 
            "put", &CoapClientConnectionWrapper::put
        );
    // clang-format on
    auto connection_type = lua_.new_usertype<ConnectionWrapper>("connection");

    auto imgui_tbl = lua_["imgui"].get_or_create<sol::table>();
    imgui_tbl.set_function("colorEdit3", [](const char *label, std::array<float, 3> vals) {
        ImGui::ColorEdit3(label, &vals[0]);
        return vals;
    });
    imgui_tbl.set_function("text", [](const char *text) { ImGui::Text("%s", text); });
    imgui_tbl.set_function("button", [](const char *label) { return ImGui::Button(label); });

    auto res = lua_.script_file(plugin_file.generic_string());
    if (!res.valid())
    {
        sol::error error = res;
        spdlog::error("{}", error.what());
    }

    sol::table plugin = lua_["plugin"];
    name_ = plugin.get<std::string>("name");
    description_ = plugin.get<std::string>("description");
    setupConnections(manager);
    setupActions();

    lua_["setup"]();
}
LuaPlugin::~LuaPlugin()
{}

void LuaPlugin::setupConnections(con::Manager &manager)
{
    const sol::table wanted_cons = lua_["plugin"]["connections"];
    for (const auto &key_value_pair : wanted_cons)
    {
        const std::string var_name = key_value_pair.first.as<std::string>();
        const sol::table decr_table = key_value_pair.second;
        const auto type_str = decr_table["type"].get<std::string>();
        const auto name_str = decr_table["name"].get<std::string>();

        if (type_str == "coap-client")
        {
            auto client = std::make_shared<con::CoapClient>(manager);
            client->setReadableName(name_str);
            con::CoapClient::Options opts;
            opts.server_uri = decr_table["ip"].get<std::string>();
            opts.server_port = decr_table["port"].get<uint16_t>();
            client->setOptions(opts);

            lua_[var_name] =
                std::make_unique<CoapClientConnectionWrapper>(manager.ctx(), client, decr_table["onReceive"]);
            connections_.emplace_back(std::move(client));
        }
        else if (type_str == "serial")
        {
            auto conn = std::make_shared<con::Serial>(manager);
            conn->setReadableName(name_str);
            con::Serial::Options opts{};
            opts.port = decr_table["port"].get<std::string>();
            opts.baud_rate = decr_table["baudrate"].get<uint32_t>();
            opts.character_size = 8;
            opts.flow_control = con::Serial::FlowControl::none;
            opts.parity = con::Serial::Parity::none;
            opts.stop_bits = con::Serial::StopBits::one;
            conn->setOptions(opts);

            lua_[var_name] = std::make_unique<ConnectionWrapper>(manager.ctx(), conn, decr_table["onReceive"]);

            connections_.emplace_back(std::move(conn));
        }
    }
}

void LuaPlugin::setupActions()
{
    const sol::table wanted_actions = lua_["plugin"]["actions"];
    for (const auto &key_value_pair : wanted_actions)
    {
        const sol::table action_tbl = key_value_pair.second;

        actions_.emplace_back(std::make_unique<LuaActionItem>(action_tbl["gui"].get<sol::function>()));
    }
}

const std::string &LuaPlugin::name() const
{
    return name_;
}
const std::string &LuaPlugin::description() const
{
    return description_;
}

const std::vector<Plugin::ConnDescriptor> &LuaPlugin::getConnections() const
{
    return connections_;
}
const std::vector<std::unique_ptr<ActionItem>> &LuaPlugin::getActionItems() const
{
    return actions_;
}
} // namespace dev::plg
