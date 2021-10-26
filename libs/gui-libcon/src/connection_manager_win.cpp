#include "gui-libcon/connection_manager_win.hpp"
#include <fmt/format.h>
namespace dev::gui
{
ConnectionManagerWin::ConnectionManagerWin(const std::shared_ptr<con::Manager> &manager)
    : Window{fmt::format("Connection Manager")}
    , manager_{manager}
{}
ConnectionManagerWin::~ConnectionManagerWin()
{}

void ConnectionManagerWin::updateContent()
{}
} // namespace dev::gui
