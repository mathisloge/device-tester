#include "gui-libcon/tcp_client_win.hpp"
#include <fmt/format.h>
namespace dev::gui
{
TcpClientWin::TcpClientWin(const std::shared_ptr<con::TcpClient> &client)
    : Window{fmt::format("UdpConnection###UDP{}", reinterpret_cast<std::uintptr_t>(client.get()))}
    , client_{client}
{}
TcpClientWin::~TcpClientWin()
{}
void TcpClientWin::updateContent()
{}
} // namespace dev::gui
