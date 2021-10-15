#include "application.hpp"
#include <exception>
#include <fmt/format.h>
#include <imgui.h>
#include "version.hpp"

Application::Application()
    : run_render_loop_{false}
{}
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
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                appExit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Connections"))
        {

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Application::renderImgui()
{
    menuBar();
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
    return fmt::format("Device Tester {} - {}", dev::kVersion, dev::kGitRef);
}
