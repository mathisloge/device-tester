#include "libcon/connection.hpp"
#include <string>
namespace dev::con
{
class Connection::BaseImpl final
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

Connection::Connection()
    : base_impl_{std::make_unique<BaseImpl>()}
{}
Connection::~Connection()
{}

const std::string &Connection::readableName() const
{
    return base_impl_->readable_name_;
}
void Connection::setReadableName(std::string_view name)
{
    base_impl_->readable_name_ = name;
}

} // namespace dev::con
