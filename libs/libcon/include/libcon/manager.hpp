#pragma once
#include <memory>
#include "libcon_export.hpp"

namespace dev::con
{
class ManagerImpl;
class LIBCON_EXPORT Manager final
{
  public:
    Manager();
    ~Manager();
    ManagerImpl &impl();

  private:
    std::unique_ptr<ManagerImpl> impl_;
};
} // namespace dev::con
