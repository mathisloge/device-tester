#pragma once
#include <string>
#include <libcon/manager.hpp>
#include <gui-common/window_manager.hpp>
#include <gui-libcon/connection_manager_win.hpp>
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
    dev::gui::WindowManager win_manager_;
    std::shared_ptr<dev::con::Manager> connection_manager_;
    dev::gui::ConnectionManagerWin connection_manager_win_;

  protected:
    static std::string getTitle();
};
