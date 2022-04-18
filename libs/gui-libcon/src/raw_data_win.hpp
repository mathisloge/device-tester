#pragma once
#include <chrono>
#include <span>
#include <string>
#include <vector>
#include <boost/signals2.hpp>
#include <gui-common/window.hpp>

#include <imgui.h>
#include <imgui_memory_editor.h>

namespace dev::gui
{
class RawDataWin final : public Window
{
    enum class DataType
    {
        send,
        received
    };
    struct Data
    {
        std::chrono::time_point<std::chrono::system_clock> ts;
        DataType type;
        std::string data;
    };

  public:
    using SendSignal = boost::signals2::signal<void(std::span<uint8_t>)>;

  public:
    RawDataWin(const std::string &title, const intptr_t unique_id);
    ~RawDataWin();

    boost::signals2::connection connectSend(const SendSignal::slot_type &sub);
    void addData(std::span<uint8_t> data);

  private:
    void updateContent() override;

  private:
    MemoryEditor mem_edit_;
    SendSignal send_sig_;
    std::string input_buf_;
    std::vector<uint8_t> data_;
    bool auto_scroll_;
    bool use_mem_editor_;
};

} // namespace dev::gui
