#include "lua_gui_bindings.hpp"
#include <imgui.h>
#include <implot.h>
namespace dev::plg
{
// utility structure for realtime plot
struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size)
    {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y)
    {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else
        {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase()
    {
        if (Data.size() > 0)
        {
            Data.shrink(0);
            Offset = 0;
        }
    }

    void plotLine(const char *label)
    {
        ImPlot::PlotLine(label, &Data[0].x, &Data[0].y, Data.size(), Offset, 2 * sizeof(float));
    }
};

// utility structure for realtime plot
struct RollingBuffer
{
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer()
    {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y)
    {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

void registerImGuiFunctions(sol::table ns)
{
    ns.set_function("colorPicker3", [](const char *label, std::array<float, 3> vals) {
        const bool changed = ImGui::ColorPicker3(label, &vals[0]);
        return std::tuple<bool, std::array<float, 3>>{changed, vals};
        // return vals;
    });
    ns.set_function("colorEdit3", [](const char *label, std::array<float, 3> vals) {
        const bool changed = ImGui::ColorEdit3(label, &vals[0]);
        return std::tuple<bool, std::array<float, 3>>{changed, vals};
    });
    ns.set_function("text", [](const char *text) { ImGui::Text("%s", text); });
    ns.set_function("button", [](const char *label) { return ImGui::Button(label); });
}

void registerImPlotFunctions(sol::table ns)
{
    ns.set_function("beginPlot", [](const char *label) { return ImPlot::BeginPlot(label); });
    ns["endPlot"] = ImPlot::EndPlot;
    ns["setupAxisLimits"] = ImPlot::SetupAxisLimits;
    
    auto scr_buf_type = ns.new_usertype<ScrollingBuffer>("ScrollingBuffer", sol::constructors<ScrollingBuffer(int)>());
    scr_buf_type["addPoint"] = &ScrollingBuffer::AddPoint;
    scr_buf_type["erase"] = &ScrollingBuffer::Erase;
    scr_buf_type["plotLine"] = &ScrollingBuffer::plotLine;

}
} // namespace dev::plg
