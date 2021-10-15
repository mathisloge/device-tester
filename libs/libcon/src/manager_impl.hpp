#pragma once
#include <asio.hpp>
namespace dev::con
{
class ManagerImpl final
{
  public:
    ManagerImpl();
    ~ManagerImpl();
    asio::io_context &ctx();

  private:
    asio::io_context io_;
};
} // namespace dev::con
