#pragma once
#include <asio.hpp>
#include "writer.hpp"
class BasicClient : public Writer
{
  public:
    BasicClient(asio::io_context &io_context);
    virtual ~BasicClient();

  protected:
    asio::strand<asio::any_io_executor> strand_;
};
