#pragma once
#include <memory>
#include <libcon/tcp_client.hpp>
#include <gui-common/window.hpp>
namespace dev::gui
{
class TcpClientWin final : public Window
{
  public:
    explicit TcpClientWin(const std::shared_ptr<con::TcpClient> &client);
    ~TcpClientWin();

  private:
    void updateContent() override;

  private:
    std::shared_ptr<con::TcpClient> client_;
};
} // namespace dev::gui
