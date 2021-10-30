#include "gui-libcon/tcp_server_win.hpp"
#include <array>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
namespace dev::gui
{
TcpServerWin::TcpServerWin(con::Manager &manager)
    : server_{std::make_shared<con::TcpServer>(manager)}
{
    opts_ = server_->options();
}
TcpServerWin::~TcpServerWin()
{}

const std::string &TcpServerWin::title() const
{
    static const std::string kEmptyName = "[NEW] TCP-Server";
    if (server_->readableName().empty() && server_->connectionReadableName().empty())
        return kEmptyName;
    return server_->readableName().empty() ? server_->connectionReadableName() : server_->readableName();
}
void TcpServerWin::drawTabSettings()
{
    // ImGui::InputText("Server-Protocol", &opts_.protocol);
    constexpr std::array<const char *, 3> items{"none", "IPv4", "IPv6"};
    if (static_cast<size_t>(opts_.protocol) >= items.size())
        opts_.protocol = con::IpProtocol::none;
    if (ImGui::BeginCombo("IP-Protocol", items[static_cast<size_t>(opts_.protocol)]))
    {
        for (size_t i = 0; i < items.size(); i++)
        {
            if (ImGui::Selectable(items[i], i == static_cast<size_t>(opts_.protocol)))
                opts_.protocol = con::IpProtocol(i);
        }
        ImGui::EndCombo();
    }

    ImGui::InputScalar("Server-Port", ImGuiDataType_U16, &opts_.port);

    if (ImGui::Button("Apply"))
    {
        server_->setOptions(opts_);
    }
    ImGui::SameLine();
    if (server_->isRunning())
    {
        if (ImGui::Button("Stop"))
            server_->stop();
    }
    else
    {
        if (ImGui::Button("Start"))
        {
            server_->setOptions(opts_);
            server_->start();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        opts_ = server_->options();
    }
}

} // namespace dev::gui
