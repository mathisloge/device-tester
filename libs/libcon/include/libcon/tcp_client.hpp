#pragma once
#include <memory>
#include <string>
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT TcpClient : public Connection
{
  public:
    struct Options
    {
        std::string server;
        std::string service;
    };

  public:
    TcpClient(Manager &manager);
    virtual ~TcpClient();

    void setOptions(const Options &opts);
    const Options &options() const;
    bool isConnected() const;
    void connect();
    void disconnect();

    boost::signals2::connection connectOnReceive(const ReiceiveSignal::slot_type &sub) override;
    void send(std::span<uint8_t> data) override;

    std::string generateReadableName() const override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
