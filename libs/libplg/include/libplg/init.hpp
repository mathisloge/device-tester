#pragma once
#include "libplg_export.hpp"
struct ImGuiContext;
struct ImPlotContext;
namespace dev::plg
{
void LIBPLG_EXPORT SetImGuiContext(ImGuiContext *ctx, ImPlotContext* implot);
}
