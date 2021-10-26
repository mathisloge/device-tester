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
    bool open() const;
    bool &open();

  protected:
    virtual void updateContent() = 0;

  protected:
    std::string title_;
    bool visibile_;
    int flags_;
};

} // namespace dev::gui
