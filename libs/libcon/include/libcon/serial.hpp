#pragma once
#include <cstdint>
#include <string>
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT Serial : public Connection
{
  public:
    enum class Parity
    {
        none,
        odd,
        even
    };
    enum class FlowControl
    {
        none,
        software,
        hardware
    };
    enum class StopBits
    {
        one,
        onepointfive,
        two
    };
    struct Options
    {
        std::string port;
        uint32_t baud_rate;
        unsigned short character_size;
        Parity parity;
        FlowControl flow_control;
        StopBits stop_bits;
    };

  public:
    Serial(Manager &manager);
    virtual ~Serial();

    const Options &options() const;
    void setOptions(const Options &opts);

    void send(std::span<uint8_t> data) override;
    boost::signals2::connection connectOnReceive(const ReiceiveSignal::slot_type &sub) override;
    std::string generateReadableName() const override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
