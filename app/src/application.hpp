#pragma once
#include <string>
#include <libcon/manager.hpp>
#include <gui-libcon/tcp_client_win.hpp>
class Application
{
  public:
    Application();
    virtual ~Application();
    void run();

  protected:
    virtual bool setup() = 0;
    virtual void renderLoop() = 0;
    void appExit();
    bool shouldRun() const;
    void renderImgui(); //! have to be called by impl

  private:
    void menuBar();

  private:
    bool run_render_loop_;
    dev::con::Manager manager_;
    std::unique_ptr<dev::gui::TcpClientWin> win_;

  protected:
    static std::string getTitle();
};
