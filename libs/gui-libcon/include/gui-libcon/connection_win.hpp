#pragma once
#include <string>

namespace dev::gui
{
class ConnectionWin
{
  public:
    ConnectionWin();
    virtual ~ConnectionWin();
    virtual const std::string &title() const = 0;
    virtual void drawTabSettings() = 0;
};
} // namespace dev::gui
