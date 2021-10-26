#pragma once
#include <chrono>
#include <span>
#include <string>
#include <vector>
#include <gui-common/window.hpp>

namespace dev::gui
{
class RawDataWin final : public Window
{
    struct Data
    {
        std::chrono::time_point<std::chrono::system_clock> ts;
        std::string data;
    };

  public:
    RawDataWin();
    ~RawDataWin();

    void addData(std::span<uint8_t> data);

  private:
    void updateContent() override;

  private:
    std::string input_buf_;

    std::vector<Data> data_;
};

} // namespace dev::gui
