project(gui-libcon)

add_library(gui-libcon STATIC
    src/gui_elements.hpp
    src/gui_elements.cpp
    src/tcp_client_win.cpp
    src/tcp_server_win.cpp
    src/connection_manager_win.cpp
    src/connection_win.cpp
    src/raw_data_win.cpp
    src/udp_con_win.cpp
    src/udp_con_win.hpp
    src/coap_client_win.cpp
    src/coap_client_win.hpp
    src/serial_win.cpp
    src/serial_win.hpp
)
target_include_directories(gui-libcon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(gui-libcon 
    PUBLIC 
        fmt::fmt 
        gui-common 
        libcon
    PRIVATE
        imgui-club
)

set_target_properties(gui-libcon PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)
