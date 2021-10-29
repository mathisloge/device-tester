project(gui-libcon)

add_library(gui-libcon STATIC
    src/tcp_client_win.cpp
    src/connection_manager_win.cpp
    src/connection_win.cpp
)
target_include_directories(gui-libcon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(gui-libcon PUBLIC fmt::fmt gui-common libcon)

set_target_properties(gui-libcon PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
