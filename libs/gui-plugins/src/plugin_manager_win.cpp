#include <filesystem>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <nfd.h>
#include <gui-plugins/plugin_manager_win.hpp>
namespace dev::gui
{

PluginManagerWin::PluginManagerWin()
    : Window{"Plugin Manager", ImGuiWindowFlags_MenuBar}
{
    plugin_.addPlugin("lua/plugins/test.lua");
}

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
                        plugin_.addPlugin(plugin_path);
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
