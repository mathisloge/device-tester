project(gui-common)

add_library(gui-common STATIC
    src/fonts.cpp
    src/window.cpp
)
target_include_directories(gui-common PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(gui-common PUBLIC imgui::imgui)

set_target_properties(gui-common PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
