add_library(gui-plugins STATIC
    src/plugin_test.cpp
)
target_include_directories(gui-plugins PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(gui-plugins PUBLIC 
    fmt::fmt 
    gui-common
    lua-base
)

set_target_properties(gui-plugins PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)
