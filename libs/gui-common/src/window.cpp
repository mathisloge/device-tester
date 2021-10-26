#include "gui-common/window.hpp"
#include <imgui.h>
namespace dev::gui
{

Window::Window(const std::string &title, int flags)
    : title_{title}
    , flags_{flags}
    , visibile_{true}
{}
Window::~Window()
{}
void Window::update()
{
    if (!visibile_)
        return;
    if (!ImGui::Begin(title_.c_str(), &visibile_, flags_))
    {
        ImGui::End();
        return;
    }
    updateContent();
    ImGui::End();
}

bool Window::open() const
{
    return visibile_;
}
bool &Window::open()
{
    return visibile_;
}

} // namespace dev::gui
