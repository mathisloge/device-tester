#include "gui-libcon/tcp_client_win.hpp"
#include <fmt/format.h>
namespace dev::gui
{
TcpClientWin::TcpClientWin(const std::shared_ptr<con::TcpClient> &client)
    : client_{client}
{}
TcpClientWin::~TcpClientWin()
{}

const std::string &TcpClientWin::title() const
{
    return client_->readableName().empty() ? client_->connectionReadableName() : client_->readableName();
}
void TcpClientWin::drawTabSettings()
{}

} // namespace dev::gui
