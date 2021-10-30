#pragma once
#include <thread>
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
    void work();

  private:
    bool run_;
    asio::io_context io_;
    std::thread runner_;
};
} // namespace dev::con
