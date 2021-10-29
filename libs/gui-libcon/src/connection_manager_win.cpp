#include "gui-libcon/connection_manager_win.hpp"
#include <fmt/format.h>
#include <imgui.h>
namespace dev::gui
{
ConnectionManagerWin::ConnectionManagerWin(const std::shared_ptr<con::Manager> &manager)
    : Window{fmt::format("Connection Manager")}
    , manager_{manager}
    , current_{nullptr}
{}
ConnectionManagerWin::~ConnectionManagerWin()
{}

void ConnectionManagerWin::updateContent()
{
    {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);
        for (const auto &c : connections_)
        {
            if (ImGui::Selectable(c->title().c_str(), c == current_))
            {
                current_ = c;
            }
        }
        ImGui::EndChild();
    }
    ImGui::SameLine();

    if (current_)
    {
        ImGui::BeginGroup();
        ImGui::BeginChild("item view",
                          ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        ImGui::Text(current_->title().c_str());
        ImGui::Separator();
        if (ImGui::BeginTabBar("##ConManagerTabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Settings"))
            {
                current_->drawTabSettings();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Details"))
            {
                ImGui::Text("ID: 0123456789");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();
        if (ImGui::Button("Revert"))
        {}
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {}
        ImGui::EndGroup();
    }
}
} // namespace dev::gui
