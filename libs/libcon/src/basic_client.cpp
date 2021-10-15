#include "basic_client.hpp"

BasicClient::BasicClient(asio::io_context &io_context)
    : strand_{asio::make_strand(io_context)}
{}
BasicClient::~BasicClient()
{}
