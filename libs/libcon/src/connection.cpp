#include "libcon/connection.hpp"
#include <string>
namespace dev::con
{
class BaseConnection::BaseImpl final
{
  public:
    BaseImpl()
        : readable_name_{}
    {}
    ~BaseImpl()
    {}

  public:
    std::string readable_name_;
};

BaseConnection::BaseConnection()
    : base_impl_{std::make_unique<BaseImpl>()}
{}
BaseConnection::~BaseConnection()
{}

const std::string &BaseConnection::readableName() const
{
    return base_impl_->readable_name_;
}
void BaseConnection::setReadableName(const std::string_view name)
{
    base_impl_->readable_name_ = name;
}

Connection::Connection()
{}
Connection::~Connection()
{}
} // namespace dev::con
