#pragma once
#include "base_window.hpp"
#include "../widgets/raw_text_output.hpp"
//#include "../widgets/raw_mem_viewer.hpp"
#include "../widgets/plot_throughput.hpp"
#include "../../devices/device_connection.hpp"
namespace gui
{
    class ConnectionWin : public BaseWindow
    {
    public:
        explicit ConnectionWin(const std::string &name, DeviceConnection &device_connection);

    protected:
        virtual void drawConnectionOverview() = 0;
        virtual void drawConnectionRawInput() = 0;
        virtual void drawConnectionDetails() = 0;
        virtual void drawConnectionSettings() = 0;
        void drawRawOutput();

    private:
        void drawContent() override;

    protected:
        std::string error_str_;

    private:
        RawTextOutput raw_text_;
       //RawMemViewer raw_mem_viewer_;
        PlotThroughput data_throughput_;
    };
} // namespace gui
