#include "gui-libcon/connection_manager_win.hpp"
#include <fmt/format.h>
#include <imgui.h>
#include "coap_client_win.hpp"
#include "serial_win.hpp"
#include "tcp_client_win.hpp"
#include "tcp_server_win.hpp"
#include "udp_con_win.hpp"
#include <gui-common/IconsFontAwesome4.hpp>
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

void ConnectionManagerWin::registerConnection(const std::shared_ptr<con::CoapClient> &con)
{
    connections_.emplace(std::make_shared<CoapClientWin>(con, win_manager_));
}
void ConnectionManagerWin::registerConnection(const std::shared_ptr<con::Serial> &con)
{
    connections_.emplace(std::make_shared<SerialWin>(con, win_manager_));
}
void ConnectionManagerWin::registerConnection(const std::shared_ptr<con::TcpClient> &con)
{}
void ConnectionManagerWin::registerConnection(const std::shared_ptr<con::TcpServer> &con)
{}
void ConnectionManagerWin::registerConnection(const std::shared_ptr<con::UdpConnection> &con)
{}

void ConnectionManagerWin::updateContent()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Connection"))
        {
            if (ImGui::MenuItem("Serial"))
            {
                current_ = *connections_.emplace(std::make_shared<SerialWin>(*manager_, win_manager_)).first;
            }
            if (ImGui::MenuItem("TCP"))
            {
                current_ = *connections_.emplace(std::make_shared<TcpClientWin>(*manager_, win_manager_)).first;
            }
            if (ImGui::MenuItem("TCP-Server"))
            {
                current_ = *connections_.emplace(std::make_shared<TcpServerWin>(*manager_, win_manager_)).first;
            }
            if (ImGui::MenuItem("UDP-Connection"))
            {
                current_ = *connections_.emplace(std::make_shared<UdpConWin>(*manager_, win_manager_)).first;
            }
            if (ImGui::MenuItem("COAP Client"))
            {
                current_ = *connections_.emplace(std::make_shared<CoapClientWin>(*manager_, win_manager_)).first;
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
        ImGui::BeginChild("item view");
        ImGui::Text(current_->title().c_str());
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH))
        {
            connections_.erase(current_);
            current_ = nullptr;
        }
        ImGui::Separator();
        if (current_ && ImGui::BeginTabBar("##ConManagerTabs", ImGuiTabBarFlags_None))
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
            current_->drawCustomTabs();
            ImGui::EndTabBar();
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }
}
} // namespace dev::gui
