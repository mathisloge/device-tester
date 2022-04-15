#include "coap_client_win.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
namespace dev::gui
{

CoapClientWin::CoapClientWin(con::Manager &manager, WindowManager &win_manager)
    : win_manager_{win_manager}
    , client_{std::make_shared<con::CoapClient>(manager)}
    , opts_{client_->options()}
    , raw_win_{nullptr}

{}
CoapClientWin::~CoapClientWin()
{}

const std::string &CoapClientWin::title() const
{
    static const std::string kEmptyName = "[NEW] COAP Client";
    if (client_->readableName().empty())
        return kEmptyName;
    return client_->readableName();
}

void CoapClientWin::drawTabSettings()
{
    if (ImGui::InputText("Name", &client_name_))
        client_->setReadableName(client_name_);
    ImGui::InputText("Server-URI", &opts_.server_uri);
    ImGui::InputScalar("Port", ImGuiDataType_U16, &opts_.server_port);

    if (ImGui::Button("Apply"))
    {
        client_->setOptions(opts_);
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        opts_ = client_->options();
    }
}
void CoapClientWin::drawTabDetails()
{
    if (ImGui::Button("GET well-known"))
    {
        client_->getWellKnown();
    }
    if(ImGui::Button("TEST")) {
        client_->makeRequest(con::CoapClient::Method::m_put, "test", 0);
    }
}

} // namespace dev::gui
