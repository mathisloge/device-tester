#pragma once
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
        unsigned short server_port;
    };

  public:
    TcpClient(Manager &manager);
    virtual ~TcpClient();
    void setOptions(const Options &opts);

    void send(std::span<uint8_t> data) override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
