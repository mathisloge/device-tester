#include <filesystem>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <nfd.h>
#include <gui-plugins/plugin_manager_win.hpp>
namespace dev::gui
{

PluginManagerWin::PluginManagerWin(std::shared_ptr<dev::con::Manager> connection_manager)
    : Window{"Plugin Manager", ImGuiWindowFlags_MenuBar}
    , connection_manager_{connection_manager}
{}

PluginManagerWin::~PluginManagerWin()
{}

void PluginManagerWin::updateContent()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Plugins"))
        {
            if (ImGui::MenuItem("Load lua"))
            {
                nfdchar_t *outPath = NULL;
                const auto def_path{std::filesystem::current_path() / "lua"};
                nfdresult_t result = NFD_OpenDialog("lua", def_path.string().c_str(), &outPath);

                if (result == NFD_OKAY)
                {
                    const std::filesystem::path plugin_path{outPath};
                    if (std::filesystem::is_regular_file(plugin_path) && plugin_path.extension() == ".lua")
                    {
                        const auto cons = manager_.loadLuaPlugin(plugin_path).getConnections(*connection_manager_);
                    }
                    free(outPath);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

} // namespace dev::gui
