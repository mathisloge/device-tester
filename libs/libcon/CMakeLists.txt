find_package(asio CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(libcoap CONFIG REQUIRED)

add_library(libcon 
    src/manager.cpp
    src/manager_impl.hpp
    src/manager_impl.cpp
    src/basic_client.hpp
    src/basic_client.cpp
    src/connection.cpp
    src/tcp_client.cpp
    src/tcp_server.cpp
    src/udp_connection.cpp
    src/serial.cpp
    src/ring_buffer.hpp
    src/writer.cpp
    src/receiver.cpp
    src/coap_client.cpp
    src/coap_state.hpp
    src/coap_state.cpp
)
target_link_libraries(libcon 
    PRIVATE 
        libcoap::coap-3
        Boost::headers
    PUBLIC
        fmt::fmt
        asio::asio
)
generate_export_header(libcon EXPORT_FILE_NAME libcon_export.hpp)
target_include_directories(libcon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(libcon PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)

install(TARGETS libcon)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libcon_export.hpp" TYPE INCLUDE)

if(WIN32)
    target_compile_definitions(libcon PUBLIC _WIN32_WINNT=0x0A00)
endif()
