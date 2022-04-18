#include "gui-common/fonts.hpp"
#include <filesystem>
#include "gui-common/IconsFontAwesome4.hpp"
namespace dev::gui
{
ImFont *Fonts::default_font = nullptr;
ImFont *Fonts::roboto = nullptr;
ImFont *Fonts::roboto_mono = nullptr;

void Fonts::loadAll()
{
    loadFont(FontType::roboto);
    loadFont(FontType::roboto_mono);
}
void Fonts::loadFont(FontType type)
{
    ImGuiIO &io = ImGui::GetIO();
    static const std::filesystem::path base_path{std::filesystem::path("resources") / "fonts"};
    switch (type)
    {
    case FontType::roboto: {
        roboto = io.Fonts->AddFontFromFileTTF((base_path / "Roboto-Regular.ttf").generic_string().c_str(), 16.0f);
        static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        io.Fonts->AddFontFromFileTTF(
            (base_path / FONT_ICON_FILE_NAME_FA).generic_string().c_str(), 16.0f, &icons_config, icons_ranges);
        default_font = roboto;
        break;
    }
    case FontType::roboto_mono:
        roboto_mono =
            io.Fonts->AddFontFromFileTTF((base_path / "RobotoMono-Regular.ttf").generic_string().c_str(), 18.0f);
        break;
    }
}

} // namespace dev::gui
