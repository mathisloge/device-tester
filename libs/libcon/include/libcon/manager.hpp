#pragma once
#include <memory>
#include <asio.hpp>
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
    asio::io_context &ctx();

  private:
    std::unique_ptr<ManagerImpl> impl_;
};
} // namespace dev::con
