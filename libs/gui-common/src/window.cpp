#include "gui-common/window.hpp"
#include <imgui.h>
namespace dev::gui
{

Window::Window(const std::string &title, int flags)
    : title_{title}
    , flags_{flags}
{}
Window::~Window()
{}
void Window::update()
{
    if (!ImGui::Begin(title_.c_str(), &state_, flags_))
    {
        ImGui::End();
        return;
    }
    updateContent();
    ImGui::End();
}
void Window::setOpen(bool is_open)
{
    state_ = is_open;
}

} // namespace dev::gui
