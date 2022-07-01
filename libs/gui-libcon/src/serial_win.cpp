#include "serial_win.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
namespace dev::gui
{
SerialWin::SerialWin(con::Manager &manager, WindowManager &win_manager)
    : win_manager_{win_manager}
    , connection_{std::make_shared<con::Serial>(manager)}
    , raw_win_{nullptr}
    , opts_{connection_->options()}
{}

SerialWin::SerialWin(const std::shared_ptr<con::Serial> &connection, WindowManager &win_manager)
    : win_manager_{win_manager}
    , connection_{connection}
    , raw_win_{nullptr}
    , opts_{connection->options()}
{}
SerialWin::~SerialWin()
{}

const std::string &SerialWin::title() const
{
    static const std::string kEmptyName = "[NEW] Serial";
    if (connection_->readableName().empty())
        return kEmptyName;
    return connection_->readableName();
}

void SerialWin::drawTabSettings()
{
    static constexpr std::array<std::string_view, 3> kNamesParity{"none", "odd", "even"};
    static constexpr std::array<std::string_view, 3> kNamesStopBits{"1", "1.5", "2"};
    static constexpr std::array<std::string_view, 3> kNamesFlowControl{"none", "software", "hardware"};

    if (ImGui::InputText("Name", &name_))
        connection_->setReadableName(name_);

    ImGui::InputText("Port", &opts_.port);
    ImGui::InputScalar("Baudrate", ImGuiDataType_U32, &opts_.baud_rate);
    ImGui::InputScalar("Character size", ImGuiDataType_U16, &opts_.character_size);

    const int parity = static_cast<int>(opts_.parity);
    if (ImGui::BeginCombo("Parity",
                          (parity >= 0 && parity <= static_cast<int>(con::Serial::Parity::even))
                              ? kNamesParity[parity].data()
                              : "unknown"))
    {
        for (int i = 0; i < kNamesParity.size(); i++)
        {
            if (ImGui::Selectable(kNamesParity[i].data(), parity == i))
                opts_.parity = con::Serial::Parity(i);
        }
        ImGui::EndCombo();
    }
    const int stop_bits = static_cast<int>(opts_.stop_bits);
    if (ImGui::BeginCombo("Stop bits",
                          (stop_bits >= 0 && stop_bits <= static_cast<int>(con::Serial::StopBits::two))
                              ? kNamesStopBits[stop_bits].data()
                              : "unknown"))
    {
        for (int i = 0; i < kNamesStopBits.size(); i++)
        {
            if (ImGui::Selectable(kNamesStopBits[i].data(), stop_bits == i))
                opts_.stop_bits = con::Serial::StopBits(i);
        }
        ImGui::EndCombo();
    }
    const int flow_control = static_cast<int>(opts_.flow_control);
    if (ImGui::BeginCombo("Flow control",
                          (flow_control >= 0 && flow_control <= static_cast<int>(con::Serial::FlowControl::hardware))
                              ? kNamesFlowControl[flow_control].data()
                              : "unknown"))
    {
        for (int i = 0; i < kNamesFlowControl.size(); i++)
        {
            if (ImGui::Selectable(kNamesFlowControl[i].data(), flow_control == i))
                opts_.flow_control = con::Serial::FlowControl(i);
        }
        ImGui::EndCombo();
    }

    if (!connection_->isOpen())
    {
        if (ImGui::Button("Connect"))
        {
            connection_->open();
        }
    }
    else
    {
        if (ImGui::Button("Disconnect"))
        {
            connection_->close();
        }
    }
}

void SerialWin::drawTabDetails()
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
            raw_win_->connectSend(std::bind(&con::Serial::send, &(*connection_), std::placeholders::_1));
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
