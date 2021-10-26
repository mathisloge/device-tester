#pragma once
#include <string>
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT UdpConnection : public Connection
{
  public:
    struct Options
    {
        std::string write_address;
        unsigned short write_port;
        IpProtocol listen_protocol;
        unsigned short listen_port;
    };

  public:
    UdpConnection(Manager &manager);
    virtual ~UdpConnection();
    void setOptions(const Options &opts);

    void send(std::span<uint8_t> data) override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
