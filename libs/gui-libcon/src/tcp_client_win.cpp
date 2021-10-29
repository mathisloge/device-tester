#include "gui-libcon/tcp_client_win.hpp"
#include <fmt/format.h>
namespace dev::gui
{
TcpClientWin::TcpClientWin(con::Manager &manager)
    : client_{std::make_shared<con::TcpClient>(manager, std::bind(&TcpClientWin::onData, this, std::placeholders::_1))}
{}
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
{}

void TcpClientWin::onData(std::span<uint8_t> data)
{}

} // namespace dev::gui
