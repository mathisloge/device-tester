add_library(plugin-base INTERFACE)

target_include_directories(plugin-base INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(plugin-base INTERFACE 
    libcon
)
