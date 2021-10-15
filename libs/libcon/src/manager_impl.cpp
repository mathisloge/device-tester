#include "manager_impl.hpp"
namespace dev::con
{

ManagerImpl::ManagerImpl()
{}
ManagerImpl::~ManagerImpl()
{}

asio::io_context &ManagerImpl::ctx()
{
    return io_;
}
} // namespace dev::con
