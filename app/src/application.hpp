#pragma once
#include <string>
#include <libcon/manager.hpp>
#include "windows/raw_data_win.hpp"
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
    std::shared_ptr<dev::con::Manager> manager_;
    dev::gui::ConnectionManagerWin manager_win_;
    dev::gui::RawDataWin data_win_;

  protected:
    static std::string getTitle();
};
