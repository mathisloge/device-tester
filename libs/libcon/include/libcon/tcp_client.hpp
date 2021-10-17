#pragma once
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

  private:
    class Impl;
    Impl *impl_;
};
} // namespace dev::con
