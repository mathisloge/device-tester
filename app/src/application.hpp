#pragma once
#include <string>
#include <SDL.h>
#include <libcon/manager.hpp>
#include <gui-common/window_manager.hpp>
#include <gui-libcon/connection_manager_win.hpp>
#include <gui-plugins/plugin_manager_win.hpp>
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
    void aboutModal();

  protected:
    static void TryEnableDarkMode(SDL_Window *win);

  private:
    bool run_render_loop_;
    dev::gui::WindowManager win_manager_;
    std::shared_ptr<dev::con::Manager> connection_manager_;
    dev::gui::ConnectionManagerWin connection_manager_win_;
    dev::gui::PluginManagerWin plugin_manager_win_;

    bool show_about_modal_;

  protected:
    static std::string getTitle();
};
