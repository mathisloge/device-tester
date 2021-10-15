#pragma once
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT TcpConnection : public Connection
{
    TcpConnection(Manager &manager);
    virtual ~TcpConnection();

  private:
    class Impl;
    Impl *impl_;
};
} // namespace dev::con
