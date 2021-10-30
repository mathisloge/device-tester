#include "tcp_client_win.hpp"
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
namespace dev::gui
{
TcpClientWin::TcpClientWin(con::Manager &manager)
    : client_{std::make_shared<con::TcpClient>(manager)}
{
    client_->connectOnReceive(std::bind(&TcpClientWin::onData, this, std::placeholders::_1));
    opts_ = client_->options();
}
TcpClientWin::~TcpClientWin()
{}

const std::string &TcpClientWin::title() const
{
    static const std::string kEmptyName = "[NEW] TCP";
    if (client_->readableName().empty() && client_->connectionReadableName().empty())
        return kEmptyName;
    return client_->readableName().empty() ? client_->connectionReadableName() : client_->readableName();
}
void TcpClientWin::drawTabSettings()
{
    if (ImGui::InputText("Name", &con_name_))
        client_->setReadableName(con_name_);
    ImGui::InputText("Server-IP", &opts_.server);
    ImGui::InputText("Port or service", &opts_.service);

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
        {
            client_->setOptions(opts_);
            client_->connect();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        opts_ = client_->options();
    }
}

void TcpClientWin::drawTabDetails()
{}

void TcpClientWin::onData(std::span<uint8_t> data)
{}

} // namespace dev::gui
