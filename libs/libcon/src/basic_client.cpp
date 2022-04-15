#include "basic_client.hpp"
namespace dev::con
{
BasicClient::BasicClient(asio::io_context &io_context)
    : strand_{asio::make_strand(io_context)}
{}
BasicClient::~BasicClient()
{}
} // namespace dev::con
