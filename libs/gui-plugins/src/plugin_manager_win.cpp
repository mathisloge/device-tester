#include <filesystem>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <nfd.h>
#include <gui-common/IconsFontAwesome4.hpp>
#include <gui-plugins/plugin_manager_win.hpp>
namespace dev::gui
{

PluginManagerWin::PluginManagerWin(std::shared_ptr<con::Manager> connection_manager,
                                   ConnectionManagerWin &connection_manager_win)
    : Window{"Plugin Manager", ImGuiWindowFlags_MenuBar}
    , connection_manager_{connection_manager}
    , connection_manager_win_{connection_manager_win}
    , manager_{*connection_manager}
    , current_{nullptr}
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
                        const auto &cons = manager_.loadLuaPlugin(plugin_path).getConnections();
                        for (const auto &c : cons)
                        {
                            std::visit(
                                [this](const auto &connection) {
                                    connection_manager_win_.registerConnection(connection);
                                },
                                c);
                        }
                    }
                    free(outPath);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);
        for (const auto &plg : manager_.plugins())
        {
            ImGui::PushID(plg.get());
            if (ImGui::Selectable(plg->name().c_str(), plg == current_))
            {
                current_ = plg;
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
    }
    ImGui::SameLine();

    if (current_)
    {
        ImGui::BeginGroup();
        ImGui::BeginChild("item view");
        ImGui::Text(current_->name().c_str());
        ImGui::Separator();

        current_->getActionItems()[0]->drawGui();

        ImGui::EndChild();
        ImGui::EndGroup();
    }
}

} // namespace dev::gui
