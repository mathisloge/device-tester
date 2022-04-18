#include "serial_win.hpp"
#include <imgui.h>
namespace dev::gui
{
SerialWin::SerialWin(con::Manager &manager, WindowManager &win_manager)
    : win_manager_{win_manager}
    , connection_{std::make_shared<con::Serial>(manager)}
    , raw_win_{nullptr}
{}
SerialWin::SerialWin(const std::shared_ptr<con::Serial> &connection, WindowManager &win_manager)
    : win_manager_{win_manager}
    , connection_{connection}
    , raw_win_{nullptr}
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
{}
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
