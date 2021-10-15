#include "manager.hpp"
#include "manager_impl.hpp"

namespace dev::con
{

Manager::Manager()
    : impl_{new ManagerImpl{}}
{}

Manager::~Manager()
{
    delete impl_;
}

ManagerImpl *Manager::impl()
{
    return impl_;
}
} // namespace dev::con
