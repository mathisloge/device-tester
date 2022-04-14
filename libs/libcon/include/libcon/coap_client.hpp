#pragma once
#include <memory>
#include <string>
#include "connection.hpp"
#include "manager.hpp"

namespace dev::con
{

class LIBCON_EXPORT CoapClient : public BaseConnection
{
  public:
    static void DiscoverServers();
    static int DefaultPort();
    static int DefaultSecurePort();

  public:
    struct Options
    {
        std::string server_uri;
        unsigned short server_port;
    };

    enum class Method
    {
        m_get,
        m_post,
        m_put,
        m_delete
    };

  public:
    CoapClient(Manager &manager);
    virtual ~CoapClient();

    void applyOptions(const Options &opts);

    void getWellKnown(const std::string path = ".well-known/core");

    std::string generateReadableName() const override;

    // void req_get();
    // void req_put();
    // void req_post();
    // void req_delete();

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dev::con
