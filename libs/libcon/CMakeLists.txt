project(libcon)

find_package(asio CONFIG REQUIRED)
add_library(libcon STATIC 
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
)
target_link_libraries(libcon PRIVATE 
    asio::asio
    Boost::headers
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
)

install(FILES ${PROJECT_BINARY_DIR}/libcon_export.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(WIN32)
    target_compile_definitions(libcon PRIVATE _WIN32_WINNT=0x0A00)
endif()
