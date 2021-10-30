#include "gui-libcon/connection_manager_win.hpp"
#include <fmt/format.h>
#include <imgui.h>
#include "tcp_client_win.hpp"
#include "tcp_server_win.hpp"
namespace dev::gui
{
ConnectionManagerWin::ConnectionManagerWin(WindowManager &win_manager, const std::shared_ptr<con::Manager> &manager)
    : Window{fmt::format("Connection Manager"), ImGuiWindowFlags_MenuBar}
    , win_manager_{win_manager}
    , manager_{manager}
    , current_{nullptr}
{}
ConnectionManagerWin::~ConnectionManagerWin()
{}

void ConnectionManagerWin::updateContent()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Connection"))
        {
            if (ImGui::MenuItem("TCP"))
            {
                current_ = connections_.emplace_back(std::make_shared<TcpClientWin>(*manager_));
            }
            if (ImGui::MenuItem("TCP-Server"))
            {
                current_ = connections_.emplace_back(std::make_shared<TcpServerWin>(*manager_));
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true);
        for (const auto &c : connections_)
        {
            ImGui::PushID(&c);
            if (ImGui::Selectable(c->title().c_str(), c == current_))
            {
                current_ = c;
            }
            ImGui::PopID();
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
                current_->drawTabDetails();
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
