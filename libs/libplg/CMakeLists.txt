add_library(libplg 
    src/plugin_manager.cpp
)
target_link_libraries(libplg 
    PUBLIC
        plugin-base
    PRIVATE 
        lua-base
)
generate_export_header(libplg EXPORT_FILE_NAME libplg_export.hpp)
target_include_directories(libplg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(libplg PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)

install(TARGETS libplg)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libplg_export.hpp" TYPE INCLUDE)
