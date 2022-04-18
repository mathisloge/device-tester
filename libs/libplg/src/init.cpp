#include <imgui.h>
#include <libplg/init.hpp>
namespace dev::plg
{
void SetImGuiContext(ImGuiContext *ctx)
{
#ifndef LIBPLG_STATIC_DEFINE
#ifdef _WIN32
    ImGui::SetCurrentContext(ctx);
#endif
#endif
}
} // namespace dev::plg
