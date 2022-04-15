#pragma once
#include <memory>
#include <libcon/coap_client.hpp>
#include "raw_data_win.hpp"
#include "gui-libcon/connection_win.hpp"
#include <gui-common/window.hpp>
#include <gui-common/window_manager.hpp>

namespace dev::gui
{
class CoapClientWin final : public ConnectionWin
{
  public:
    explicit CoapClientWin(con::Manager &manager, WindowManager &win_manager);
    ~CoapClientWin();

    const std::string &title() const override;
    void drawTabSettings() override;
    void drawTabDetails() override;

  private:
    WindowManager &win_manager_;
    std::shared_ptr<con::CoapClient> client_;
    con::CoapClient::Options opts_;
    std::shared_ptr<RawDataWin> raw_win_;

    std::string client_name_;
};
} // namespace dev::gui
