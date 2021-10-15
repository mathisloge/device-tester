#pragma once
#include <asio.hpp>
class BasicClient
{
  public:
    BasicClient(asio::io_context &io_context);
    virtual ~BasicClient();

  protected:
    asio::strand<asio::any_io_executor> strand_;
};
