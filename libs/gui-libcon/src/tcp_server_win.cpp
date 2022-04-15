#include "tcp_server_win.hpp"
#include <array>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "gui_elements.hpp"
namespace dev::gui
{
TcpServerWin::TcpServerWin(con::Manager &manager, WindowManager &win_manager)
    : win_manager_{win_manager}
    , server_{std::make_shared<con::TcpServer>(manager)}
{
    opts_ = server_->options();

    server_->connectClientState([this](con::TcpServer::ClientState state, con::ITcpServerClient &client) {
        if (state == con::TcpServer::ClientState::disconnected)
        {
            auto it = client_wins_.find(&client);
            if (it != client_wins_.end())
            {
                win_manager_.unregisterWindow(it->second);
                client_wins_.erase(it);
            }
        }
    });
}
TcpServerWin::~TcpServerWin()
{}

const std::string &TcpServerWin::title() const
{
    static const std::string kEmptyName = "[NEW] TCP-Server";
    if (server_->readableName().empty())
        return kEmptyName;
    return server_->readableName();
}
void TcpServerWin::drawTabSettings()
{
    if (ImGui::InputText("Name", &con_name_))
        server_->setReadableName(con_name_);

    drawIpProtocolSelection(opts_.protocol);

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

void TcpServerWin::drawTabDetails()
{
    ImGui::Text("Number of connected clients: %i", server_->numberOfConnectedClients());
}

void TcpServerWin::drawCustomTabs()
{
    if (ImGui::BeginTabItem("Clients"))
    {
        server_->eachClient([this](con::ITcpServerClient &c) {
            if (c.readableName().empty())
                c.setReadableName(c.generateReadableName());
            const auto &title = c.readableName();
            ImGui::PushID(&c);
            if (!ImGui::CollapsingHeader(title.c_str()))
            {
                ImGui::PopID();
                return;
            }

            const auto raw_data_it = client_wins_.find(&c);

            bool has_raw_data = raw_data_it != client_wins_.end();
            if (ImGui::Checkbox("Console", &has_raw_data))
            {
                if (has_raw_data)
                {
                    const auto emplaced =
                        client_wins_.emplace(&c, std::make_shared<RawDataWin>(title, reinterpret_cast<intptr_t>(&c)));
                    if (emplaced.second)
                    {
                        auto raw_win = emplaced.first->second;
                        win_manager_.registerWindow(raw_win);
                        c.connectOnReceive(con::Connection::RxSig::slot_type{[raw_win](std::span<uint8_t> d) {
                                               raw_win->addData(d);
                                           }}.track_foreign(emplaced.first->second));
                        raw_win->connectSend(std::bind(&con::ITcpServerClient::send, &c, std::placeholders::_1));
                    }
                }
                else if (raw_data_it != client_wins_.end())
                {
                    win_manager_.unregisterWindow(raw_data_it->second);
                    client_wins_.erase(raw_data_it);
                }
            }

            ImGui::BulletText(title.c_str());
            ImGui::PopID();
        });
        ImGui::EndTabItem();
    }
}

} // namespace dev::gui
