#include "application.hpp"
#include <exception>
#include <fmt/format.h>
#include <imgui.h>
#include "version.hpp"

Application::Application()
    : run_render_loop_{false}
    , connection_manager_{std::make_shared<dev::con::Manager>()}
    , connection_manager_win_{win_manager_, connection_manager_}
    , plugin_manager_win_{std::make_shared<dev::gui::PluginManagerWin>()}
{
    win_manager_.registerWindow(plugin_manager_win_);
}

Application::~Application()
{}

void Application::run()
{
    try
    {
        setup();
    }
    catch (std::exception ex)
    {
        return;
    }
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
            ImGui::MenuItem("Plugin Manager", "", &plugin_manager_win_->open());
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                appExit();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
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

    menuBar();

    connection_manager_win_.update();
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
