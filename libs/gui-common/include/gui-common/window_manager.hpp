#pragma once
#include <memory>
#include <unordered_set>
#include "window.hpp"
namespace dev::gui
{
using WindowPtr = std::shared_ptr<Window>;
class WindowManager final
{
  public:
    WindowManager();
    ~WindowManager();

    void registerWindow(const WindowPtr &win);
    void unregisterWindow(const WindowPtr &win);
    void drawAll();

  private:
    std::unordered_set<WindowPtr> windows_;
};
} // namespace dev::gui
