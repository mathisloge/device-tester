#include "tcp_client_win.hpp"
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
namespace dev::gui
{
TcpClientWin::TcpClientWin(con::Manager &manager, WindowManager &win_manager)
    : win_manager_{win_manager}
    , client_{std::make_shared<con::TcpClient>(manager)}
    , raw_win_{nullptr}
{
    opts_ = client_->options();
}
TcpClientWin::~TcpClientWin()
{}

const std::string &TcpClientWin::title() const
{
    static const std::string kEmptyName = "[NEW] TCP";
    if (client_->readableName().empty())
        return kEmptyName;
    return client_->readableName();
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
{
    bool has_raw_win = raw_win_ != nullptr;
    if (ImGui::Checkbox("Console", &has_raw_win))
    {
        if (has_raw_win)
        {
            raw_win_ = std::make_shared<RawDataWin>(title(), reinterpret_cast<intptr_t>(this));
            client_->connectOnReceive(con::Connection::RxSig::slot_type{[this](std::span<uint8_t> d) {
                                          raw_win_->addData(d);
                                      }}.track_foreign(raw_win_));
            raw_win_->connectSend(std::bind(&con::TcpClient::send, &(*client_), std::placeholders::_1));
            win_manager_.registerWindow(raw_win_);
        }
        else if (raw_win_)
        {
            win_manager_.unregisterWindow(raw_win_);
            raw_win_ = nullptr;
        }
    }
}

} // namespace dev::gui
