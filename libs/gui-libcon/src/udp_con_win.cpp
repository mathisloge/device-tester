#include "udp_con_win.hpp"
#include <fmt/format.h>
#include <fmt/compile.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string_view>
#include <array>
namespace dev::gui
{
UdpConWin::UdpConWin(con::Manager &manager, WindowManager &win_manager)
    : win_manager_{win_manager}
    , connection_{std::make_shared<con::UdpConnection>(manager)}
    , raw_win_{nullptr}
{}
UdpConWin::~UdpConWin()
{}

const std::string &UdpConWin::title() const
{
    static const std::string kEmptyName = "[NEW] UDP";
    if (connection_->readableName().empty() && connection_->connectionReadableName().empty())
        return kEmptyName;
    return connection_->readableName().empty() ? connection_->connectionReadableName() : connection_->readableName();
}

void UdpConWin::drawTabSettings()
{
    ImGui::InputText("Write address", &opts_.write_address);
    ImGui::InputScalar("Write port", ImGuiDataType_U16, &opts_.write_port);
    ImGui::InputScalar("Listen port", ImGuiDataType_U16, &opts_.listen_port);
   

    static constexpr std::array<std::string_view, 3> kNamesProtocol{"none", "ipv4", "ipv6"};
    if (ImGui::BeginCombo("Listen protocol",
                          (opts_.listen_protocol >= con::IpProtocol::none && opts_.listen_protocol <= con::IpProtocol::ipv6)
                              ? kNamesProtocol[static_cast<int>(opts_.listen_protocol)].data()
                              : "unknown"))
    {
        for (int i = 0; i < kNamesProtocol.size(); i++)
        {

            if (ImGui::Selectable(kNamesProtocol[i].data(), opts_.listen_protocol == con::IpProtocol(i)))
                opts_.listen_protocol = con::IpProtocol(i);
        }
        ImGui::EndCombo();
    }
      
    if(ImGui::Button(fmt::format(FMT_COMPILE("{} receiving"), connection_->isReceiving() ? "Stop" : "Start" ).c_str())) {
        connection_->setReceiving(!connection_->isReceiving());
    }
    if (ImGui::Button("Reset options")) {
        opts_ = connection_->options();
    }
    if (ImGui::Button("Apply options")) {
        connection_->setOptions(opts_);
    }

}

void UdpConWin::drawTabDetails()
{
    bool has_raw_win = raw_win_ != nullptr;
    if (ImGui::Checkbox("RawDataWin", &has_raw_win))
    {
        if (has_raw_win)
        {
            raw_win_ = std::make_shared<RawDataWin>(title(), reinterpret_cast<intptr_t>(this));
            connection_->connectOnReceive(con::Connection::ReiceiveSignal::slot_type{[this](std::span<uint8_t> d) {
                                          raw_win_->addData(d);
                                      }}.track_foreign(raw_win_));
            raw_win_->connectSend(std::bind(&con::UdpConnection::send, &(*connection_), std::placeholders::_1));
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