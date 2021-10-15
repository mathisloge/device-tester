#pragma once
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT TcpClient : public Connection
{
    TcpClient(Manager &manager);
    virtual ~TcpClient();

  private:
    class Impl;
    Impl *impl_;
};
} // namespace dev::con
