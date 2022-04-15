#pragma once
#include <asio.hpp>
#include "receiver.hpp"
#include "writer.hpp"
namespace dev::con
{
class BasicClient : public Writer, public Receiver
{
  public:
    BasicClient(asio::io_context &io_context);
    virtual ~BasicClient();

  protected:
    asio::strand<asio::any_io_executor> strand_;
};
} // namespace dev::con
