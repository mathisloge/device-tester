#pragma once
#include <chrono>
#include <span>
#include <string>
#include <vector>
#include <boost/signals2.hpp>
#include <gui-common/window.hpp>

namespace dev::gui
{
class RawDataWin final : public Window
{
  enum class DataType {send, received};
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
    SendSignal send_sig_;
    std::string input_buf_;
    std::vector<Data> data_;
    bool auto_scroll_;
};

} // namespace dev::gui
