find_package(SDL2 CONFIG REQUIRED)

add_executable(device-tester 
    src/main.cpp
    src/application.hpp
    src/application.cpp
    src/application_vulkan.hpp
    src/application_vulkan.cpp
)

target_link_libraries(device-tester PRIVATE
    fmt::fmt
    SDL2::SDL2 
    SDL2::SDL2main
    imgui::imgui
    gui-common
    gui-libcon
)
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in" 
    "${CMAKE_CURRENT_BINARY_DIR}/version.hpp"
)
target_include_directories(device-tester PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(device-tester PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)

file(COPY resources DESTINATION "${shared_output_dir}")
