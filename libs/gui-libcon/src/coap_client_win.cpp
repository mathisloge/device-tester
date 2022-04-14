#include "coap_client_win.hpp"
#include <imgui.h>
namespace dev::gui
{

CoapClientWin::CoapClientWin(WindowManager &win_manager)
    : win_manager_{win_manager}
    , connection_{std::make_shared<con::CoapClient>()}
    , raw_win_{nullptr}
    , client_title_{"COAP CLIENT"}
{}
CoapClientWin::~CoapClientWin()
{}

const std::string &CoapClientWin::title() const
{
    static const std::string kEmptyName = "[NEW] COAP Client";
    if (connection_->readableName().empty())
        return kEmptyName;
    return connection_->readableName();
}

void CoapClientWin::drawTabSettings()
{
    ImGui::Text("TEST2");
    if (ImGui::Button("TEST"))
    {
        connection_->getWellKnown();
    }
}
void CoapClientWin::drawTabDetails()
{}

} // namespace dev::gui
