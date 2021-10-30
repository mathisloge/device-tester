#pragma once
#include <memory>
#include <string>
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT ITcpServerClient
{};

class LIBCON_EXPORT TcpServer
{
  public:
    struct Options
    {
        IpProtocol protocol;
        unsigned short port;
    };

  public:
    TcpServer(Manager &manager);
    virtual ~TcpServer();

    void setOptions(const Options &opts);
    const Options &options() const;
    bool isRunning() const;
    void start();
    void stop();

    const std::string &connectionReadableName() const;
    const std::string &readableName() const;
    void setReadableName(std::string_view name);

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
