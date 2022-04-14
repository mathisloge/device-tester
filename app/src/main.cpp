#include <spdlog/spdlog.h>
#include "application_vulkan.hpp"
#include "version.hpp"

int main(int, char **)
{
    spdlog::set_level(spdlog::level::trace);
    ApplicationVulkan app{};
    app.run();
    return 0;
}
