#include "raw_data_win.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <gui-common/fonts.hpp>
namespace dev::gui
{
RawDataWin::RawDataWin()
    : Window{"RawData"} // todo add id from client
{}
RawDataWin::~RawDataWin()
{}

void RawDataWin::addData(std::span<uint8_t> data)
{
    // std::copy(data.begin(), data.end(), data_.end());
}
void RawDataWin::updateContent()
{
    ImGui::PushFont(Fonts::roboto_mono);
    ImGui::InputTextMultiline("", &input_buf_, {-1.f, 0.f});
    if (ImGui::Button("send"))
    {
        std::cout << input_buf_ << std::endl;
        data_.emplace_back(Data{std::chrono::system_clock::now(), input_buf_});
        input_buf_ = "";
    }

    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGuiListClipper clipper;
        clipper.Begin(data_.size());
        while (clipper.Step())
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                ImGui::TextUnformatted(data_[i].data.c_str());
        ImGui::PopStyleVar();
    }

    ImGui::PopFont();
    ImGui::Separator();
}

} // namespace dev::gui
