#include "raw_data_win.hpp"
#include <iostream>
#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <gui-common/fonts.hpp>
namespace dev::gui
{
RawDataWin::RawDataWin(const std::string &title, const intptr_t unique_id)
    : Window{fmt::format("{} - Console###RawData{}", title, unique_id)}
    , auto_scroll_{true} // todo add id from client
{}
RawDataWin::~RawDataWin()
{}

void RawDataWin::addData(std::span<uint8_t> data)
{
    data_.emplace_back(
        Data{std::chrono::system_clock::now(), DataType::received, std::string{data.begin(), data.end()}});
}

boost::signals2::connection RawDataWin::connectSend(const SendSignal::slot_type &sub)
{
    return send_sig_.connect(sub);
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
            "ScrollingRegion", ImVec2{0, -footer_height_to_reserve}, false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
        ImGuiListClipper clipper;
        clipper.Begin(data_.size());
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                const std::string text{fmt::format(FMT_COMPILE("[{},{:%H:%M:%S}]: {}"),
                                                   data_[i].type == DataType::send ? "tx" : "rx",
                                                   data_[i].ts,
                                                   data_[i].data)};
                ImGui::TextUnformatted(text.c_str());
            }
        }

        if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::PopStyleVar();

        ImGui::EndChild();
    }
    ImGui::Separator();
    ImGui::InputText("##udptext", &input_buf_);
    ImGui::SameLine();
    if (ImGui::Button("send") && input_buf_.size() > 0)
    {
        data_.emplace_back(Data{std::chrono::system_clock::now(), DataType::send, input_buf_});
        send_sig_(std::span<uint8_t>{reinterpret_cast<uint8_t *>(input_buf_.data()), input_buf_.size()});
        input_buf_ = "";
    }
    ImGui::SameLine();
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");

    ImGui::PopFont();
}

} // namespace dev::gui
