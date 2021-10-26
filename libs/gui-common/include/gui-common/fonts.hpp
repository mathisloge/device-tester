#pragma once
#include <imgui.h>

namespace dev::gui
{
struct Fonts
{
    enum class FontType
    {
        roboto,
        roboto_mono
    };
    static ImFont *default_font;
    static ImFont *roboto;
    static ImFont *roboto_mono;

    static void loadAll();

  private:
    static void loadFont(FontType type);
};

} // namespace dev::gui
