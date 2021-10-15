#pragma once
#include <imgui.h>

namespace dev::gui
{
struct Fonts
{
    enum class FontType
    {
        roboto
    };
    static ImFont *default_font;
    static ImFont *roboto;

    static void loadAll();

  private:
    static void loadFont(FontType type);
};

} // namespace dev::gui
