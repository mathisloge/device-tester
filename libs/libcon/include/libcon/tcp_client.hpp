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
    TcpClient(Manager &manager, const onData &read_cb);
    virtual ~TcpClient();

    void setOptions(const Options &opts);
    const Options &options() const;
    bool isConnected() const;
    void connect();
    void disconnect();

    void send(std::span<uint8_t> data) override;
    const std::string &connectionReadableName() const override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
