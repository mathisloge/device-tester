#include "raw_data_win.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <gui-common/fonts.hpp>
namespace dev::gui
{
RawDataWin::RawDataWin()
    : Window{"RawData"}
    , auto_scroll_{true} // todo add id from client
{}
RawDataWin::~RawDataWin()
{}

void RawDataWin::addData(std::span<uint8_t> data)
{
    // std::copy(data.begin(), data.end(), data_.end());
}
void RawDataWin::updateContent()
{
    if (ImGui::BeginPopup("Options"))
    {
        if (ImGui::SmallButton("Clear"))
        {
            data_.clear();
        }
        ImGui::Separator();
        ImGui::Checkbox("Auto-scroll", &auto_scroll_);
        ImGui::EndPopup();
    }
    ImGui::PushFont(Fonts::roboto_mono);
    {
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild(
            "ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGuiListClipper clipper;
        clipper.Begin(data_.size());
        while (clipper.Step())
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                ImGui::TextUnformatted(data_[i].data.c_str());

        if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::PopStyleVar();

        ImGui::EndChild();
    }
    ImGui::Separator();
    ImGui::InputText("", &input_buf_);
    ImGui::SameLine();
    if (ImGui::Button("send"))
    {
        data_.emplace_back(Data{std::chrono::system_clock::now(), input_buf_});
        input_buf_ = "";
    }
    ImGui::SameLine();
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");

    ImGui::PopFont();
}

} // namespace dev::gui
