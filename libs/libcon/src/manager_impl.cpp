#include "manager_impl.hpp"
#include <spdlog/spdlog.h>
namespace dev::con
{

ManagerImpl::ManagerImpl()
    : run_{true}
    , guard_{io_.get_executor()}
    , runner_{std::bind(&ManagerImpl::work, this)}

{}
ManagerImpl::~ManagerImpl()
{
    run_ = false;
    guard_.reset();
    if (runner_.joinable())
        runner_.join();
}

asio::io_context &ManagerImpl::ctx()
{
    return io_;
}

void ManagerImpl::work()
{
    while (run_)
    {
        asio::error_code e;
        io_.run(e);
        if (e)
            spdlog::error("ManagerImpl: {}", e.message());
    }
}
} // namespace dev::con
