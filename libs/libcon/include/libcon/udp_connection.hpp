#pragma once
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

  private:
    class Impl;
    Impl *impl_;
};
} // namespace dev::con
