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
    virtual void drawTabDetails() = 0;

  protected:
    std::string con_name_;
};
} // namespace dev::gui
