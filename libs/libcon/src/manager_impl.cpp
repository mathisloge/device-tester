#include "manager_impl.hpp"
#include <spdlog/spdlog.h>
namespace dev::con
{

ManagerImpl::ManagerImpl()
    : run_{true}
    , guard_{io_.get_executor()}
{
    spdlog::set_level(spdlog::level::level_enum::debug);
    for (int i = 0; i < 10; i++)
    {
        runners_.emplace_back(std::bind(&ManagerImpl::work, this, i));
    }
}
ManagerImpl::~ManagerImpl()
{
    run_ = false;
    guard_.reset();
    for (auto &r : runners_)
    {
        if (r.joinable())
            r.join();
    }
}

asio::io_context &ManagerImpl::ctx()
{
    return io_;
}

void ManagerImpl::work(int id)
{
    spdlog::debug("ManagerImpl: starting {}", id);
    while (run_)
    {
        asio::error_code e;
        io_.run(e);
        if (e)
            spdlog::error("ManagerImpl: {}", e.message());
    }
    spdlog::debug("ManagerImpl: finished {}", id);
}
} // namespace dev::con
