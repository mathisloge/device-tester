#pragma once
#include <string>

namespace dev::gui
{
class Window
{
  public:
    explicit Window(const std::string &title, int flags = 0);
    virtual ~Window();

    void update();
    void setOpen(bool is_open);

  protected:
    virtual void updateContent() = 0;

  protected:
    std::string title_;
    bool state_;
    int flags_;
};

} // namespace dev::gui
