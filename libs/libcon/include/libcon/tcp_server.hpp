#pragma once
#include <memory>
#include <string>
#include <boost/signals2.hpp>
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{
class LIBCON_EXPORT ITcpServerClient : public Connection
{};

class LIBCON_EXPORT TcpServer : public BaseConnection
{
  public:
    struct Options
    {
        IpProtocol protocol;
        unsigned short port;
    };
    enum class ClientState
    {
        connected,
        disconnected
    };
    using ClientStateSignal = boost::signals2::signal<void(ClientState, ITcpServerClient &)>;

  public:
    TcpServer(Manager &manager);
    virtual ~TcpServer();

    void setOptions(const Options &opts);
    const Options &options() const;
    bool isRunning() const;
    void start();
    void stop();

    void sendToAll(std::span<uint8_t> data);
    int numberOfConnectedClients() const;
    void eachClient(const std::function<void(ITcpServerClient &client)> &pred);
    boost::signals2::connection connectClientState(const ClientStateSignal::slot_type &sub);
    
    std::string generateReadableName() const override;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
