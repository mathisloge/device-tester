#pragma once
#include "connection.hpp"
#include "manager.hpp"
namespace dev::con
{
class LIBCON_EXPORT UdpConnection : public Connection
{
  public:
    struct Options
    {};

  public:
    UdpConnection(Manager &manager);
    virtual ~UdpConnection();

    void setOptions(const Options& opts);

  private:
    class Impl;
    Impl *impl_;
};
} // namespace dev::con
