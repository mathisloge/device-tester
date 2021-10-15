#include "libcon/tcp_client.hpp"
#include "basic_client.hpp"
#include "manager_impl.hpp"
namespace dev::con
{

class TcpClient::Impl final : public BasicClient
{
  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl()->ctx()}
    {}
};

TcpClient::TcpClient(Manager &manager)
    : impl_{new Impl{manager}}
{}

TcpClient::~TcpClient()
{
    delete impl_;
}
} // namespace dev::con
