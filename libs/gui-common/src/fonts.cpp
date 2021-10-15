#include "gui-common/fonts.hpp"
#include <filesystem>
namespace dev::gui
{
ImFont *Fonts::default_font = nullptr;
ImFont *Fonts::roboto = nullptr;

void Fonts::loadAll()
{
    loadFont(FontType::roboto);
}
void Fonts::loadFont(FontType type)
{
    ImGuiIO &io = ImGui::GetIO();
    static const std::filesystem::path base_path{std::filesystem::path("resources") / "fonts"};
    switch (type)
    {
    case FontType::roboto:
        roboto = io.Fonts->AddFontFromFileTTF((base_path / "Roboto-Regular.ttf").generic_string().c_str(), 16.0f);
        default_font = roboto;
        break;
    }
}

} // namespace dev::gui
