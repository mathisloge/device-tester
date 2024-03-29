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
    void work(int id);

  private:
    using work_guard_type = asio::executor_work_guard<asio::io_context::executor_type>;
    bool run_;
    asio::io_context io_;
    work_guard_type guard_;
    std::vector<std::thread> runners_;
};
} // namespace dev::con
