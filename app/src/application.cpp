#include "application.hpp"
#include <exception>
#include <SDL_syswm.h>
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <libplg/init.hpp>
#include "version.hpp"

#ifdef _WIN32
#include <dwmapi.h>
#include <windows.h>
#endif

Application::Application()
    : run_render_loop_{false}
    , connection_manager_{std::make_shared<dev::con::Manager>()}
    , connection_manager_win_{win_manager_, connection_manager_}
    , plugin_manager_win_{connection_manager_, connection_manager_win_}
    , show_about_modal_{false}
{}

Application::~Application()
{}

void Application::run()
{
    try
    {
        setup();
        ImPlot::CreateContext();
    }
    catch (std::exception ex)
    {
        return;
    }
    dev::plg::SetImGuiContext(ImGui::GetCurrentContext(), ImPlot::GetCurrentContext());
    run_render_loop_ = true;
    renderLoop();
}

void Application::menuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Connection Manager", "", &connection_manager_win_.open());
            ImGui::MenuItem("Plugin Manager", "", &plugin_manager_win_.open());
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                appExit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About", "", &show_about_modal_);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Application::aboutModal()
{
    if (!show_about_modal_)
        return;
    if (!ImGui::Begin("About DuT-App",
                      &show_about_modal_,
                      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                          ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
    {

        ImGui::End();
        return;
    }
    ImGui::Text("Version: %s", dev::kVersion);
    ImGui::Text("GIT REF: %s", dev::kGitRef);
    ImGui::Text("GIT SPEC: %s", dev::kGitRefSpec);
    ImGui::Separator();
    ImGui::Text("By Mathis Logemann.");
    ImGui::Text("DuT-App is licensed under the MIT License, see LICENSE for more information.");
    ImGui::Separator();

    ImGui::End();
}

void Application::renderImgui()
{
    {
        constexpr ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                                  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                                  ImGuiWindowFlags_NoNavFocus;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("MainDockspace", nullptr, window_flags);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("MainWindowDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        ImGui::End();
    }
    aboutModal();

    menuBar();

    connection_manager_win_.update();
    plugin_manager_win_.update();
    win_manager_.drawAll();

    ImGui::ShowDemoWindow();
}

void Application::appExit()
{
    run_render_loop_ = false;
}

bool Application::shouldRun() const
{
    return run_render_loop_;
}

std::string Application::getTitle()
{
    return fmt::format("DuT-App {} - {}", dev::kVersion, dev::kGitRef);
}

void Application::TryEnableDarkMode(SDL_Window *win)
{
#ifdef _WIN32
    // dark title bar :)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(win, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    BOOL USE_DARK_MODE = true;
    BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
        hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &USE_DARK_MODE, sizeof(USE_DARK_MODE)));
#endif
}
