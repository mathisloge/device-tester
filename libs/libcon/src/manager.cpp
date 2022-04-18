#include "libcon/manager.hpp"
#include "manager_impl.hpp"

namespace dev::con
{

Manager::Manager()
    : impl_{std::make_unique<ManagerImpl>()}
{}

Manager::~Manager()
{}

ManagerImpl &Manager::impl()
{
    return *impl_;
}
asio::io_context &Manager::ctx()
{
    return impl_->ctx();
}
} // namespace dev::con
