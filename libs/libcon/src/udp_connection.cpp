#include "libcon/udp_connection.hpp"
#include "basic_client.hpp"
#include "manager_impl.hpp"
namespace dev::con
{
class UdpConnection::Impl final : public BasicClient
{
    using udp = asio::ip::udp;

  public:
    Impl(Manager &manager)
        : BasicClient{manager.impl()->ctx()}
        , socket_{strand_}
    {}

  private:
    udp::socket socket_;
    std::array<uint8_t, 65535> buffer_rx_;
    udp::endpoint listen_endpoint_;
    udp::endpoint remote_endpoint_;
    udp::endpoint write_endpoint_;
};

UdpConnection::UdpConnection(Manager &manager)
    : impl_{new Impl{manager}}
{}

UdpConnection::~UdpConnection()
{
    delete impl_;
}
} // namespace dev::con
