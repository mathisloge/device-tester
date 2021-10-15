#pragma once
#include "application.hpp"
struct SDL_Window;
struct ImGui_ImplVulkanH_Window;
class ApplicationVulkan final : public Application
{
  private:
    bool setup() override;
    void renderLoop() override;

  private:
    SDL_Window *window_;
    ImGui_ImplVulkanH_Window *wd_;
};
