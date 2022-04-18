#pragma once
#include "libplg_export.hpp"
struct ImGuiContext;

namespace dev::plg
{
void LIBPLG_EXPORT SetImGuiContext(ImGuiContext *ctx);
}
