add_library(gui-plugins STATIC
    src/plugin_manager_win.cpp
)
target_include_directories(gui-plugins PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(gui-plugins PUBLIC 
    fmt::fmt 
    unofficial::nativefiledialog::nfd
    gui-common
    libplg
)

set_target_properties(gui-plugins PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)
