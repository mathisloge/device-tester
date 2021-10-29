#include "gui-libcon/tcp_client_win.hpp"
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
namespace dev::gui
{
TcpClientWin::TcpClientWin(con::Manager &manager)
    : client_{std::make_shared<con::TcpClient>(manager, std::bind(&TcpClientWin::onData, this, std::placeholders::_1))}
{
    opts_ = client_->options();
}
TcpClientWin::~TcpClientWin()
{}

const std::string &TcpClientWin::title() const
{
    static const std::string kEmptyName = "[NEW] TCP";
    if (client_->readableName().empty() && client_->readableName().empty())
        return kEmptyName;
    return client_->readableName().empty() ? client_->connectionReadableName() : client_->readableName();
}
void TcpClientWin::drawTabSettings()
{
    ImGui::InputText("Server-IP", &opts_.server);
    ImGui::InputScalar("Server-Port", ImGuiDataType_U16, &opts_.server_port);

    if (ImGui::Button("Apply"))
    {
        client_->setOptions(opts_);
    }
    ImGui::SameLine();
    if (client_->isConnected())
    {
        if (ImGui::Button("Disconnect"))
            client_->disconnect();
    }
    else
    {
        if (ImGui::Button("Connect"))
            client_->connect();
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        opts_ = client_->options();
    }
}

void TcpClientWin::onData(std::span<uint8_t> data)
{}

} // namespace dev::gui
