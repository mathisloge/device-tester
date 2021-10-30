#pragma once
#include <memory>
#include <string>
namespace dev::gui
{
class ConnectionWin : public std::enable_shared_from_this<ConnectionWin>
{
  public:
    ConnectionWin();
    virtual ~ConnectionWin();
    virtual const std::string &title() const = 0;
    virtual void drawTabSettings() = 0;
    virtual void drawTabDetails() = 0;
    virtual void drawCustomTabs();

  protected:
    std::string con_name_;
};
} // namespace dev::gui
