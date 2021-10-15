#include "tcp_connection.hpp"
#include "basic_client.hpp"
#include "manager_impl.hpp"
namespace dev::con
{

class TcpConnection::Impl final : public BasicClient
{
  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl()->ctx()}
    {}
};

TcpConnection::TcpConnection(Manager &manager)
    : impl_{new Impl{manager}}
{}

TcpConnection::~TcpConnection()
{
    delete impl_;
}
} // namespace dev::con
