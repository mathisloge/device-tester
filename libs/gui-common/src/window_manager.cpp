#include "gui-common/window_manager.hpp"
namespace dev::gui
{
WindowManager::WindowManager()
{}
WindowManager::~WindowManager()
{}

void WindowManager::registerWindow(const WindowPtr &win)
{
    windows_.emplace(win);
}
void WindowManager::unregisterWindow(const WindowPtr &win)
{
    windows_.erase(win);
}
void WindowManager::drawAll()
{
    for (const auto &w : windows_)
        w->update();
}
} // namespace dev::gui
