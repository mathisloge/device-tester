#include "udp_con_win.hpp"
#include <array>
#include <string_view>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "gui_elements.hpp"
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
    if (connection_->readableName().empty())
        return kEmptyName;
    return connection_->readableName();
}

void UdpConWin::drawTabSettings()
{
    ImGui::InputText("Write address", &opts_.write_address);
    ImGui::InputScalar("Write port", ImGuiDataType_U16, &opts_.write_port);
    ImGui::InputScalar("Listen port", ImGuiDataType_U16, &opts_.listen_port);

    drawIpProtocolSelection(opts_.listen_protocol);

    if (ImGui::Button(fmt::format(FMT_COMPILE("{} receiving"), connection_->isReceiving() ? "Stop" : "Start").c_str()))
    {
        connection_->setReceiving(!connection_->isReceiving());
    }
    if (ImGui::Button("Reset options"))
    {
        opts_ = connection_->options();
    }
    if (ImGui::Button("Apply options"))
    {
        connection_->setOptions(opts_);
    }
}

void UdpConWin::drawTabDetails()
{
    bool has_raw_win = raw_win_ != nullptr;
    if (ImGui::Checkbox("Console", &has_raw_win))
    {
        if (has_raw_win)
        {
            raw_win_ = std::make_shared<RawDataWin>(title(), reinterpret_cast<intptr_t>(this));
            connection_->connectOnReceive(con::Connection::RxSig::slot_type{[this](std::span<uint8_t> d) {
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
