#pragma once
#include <imgui.h>

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
