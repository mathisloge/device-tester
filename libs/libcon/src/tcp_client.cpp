#include "libcon/tcp_client.hpp"
#include "basic_client.hpp"
#include "manager_impl.hpp"
namespace dev::con
{

class TcpClient::Impl final : public BasicClient
{
  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl().ctx()}
    {}
};

TcpClient::TcpClient(Manager &manager)
    : impl_{std::make_unique<Impl>(manager)}
{}

TcpClient::~TcpClient()
{}

void TcpClient::setOptions(const Options &opts)
{}

void TcpClient::send(std::span<uint8_t> data)
{}
} // namespace dev::con
