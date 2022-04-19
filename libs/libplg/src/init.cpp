#include <imgui.h>
#include <implot.h>
#include <libplg/init.hpp>
namespace dev::plg
{
void SetImGuiContext(ImGuiContext *ctx, ImPlotContext* implot)
{
#ifndef LIBPLG_STATIC_DEFINE
#ifdef _WIN32
    ImGui::SetCurrentContext(ctx);
    ImPlot::SetCurrentContext(implot);
#endif
#endif
}
} // namespace dev::plg
