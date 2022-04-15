find_package(Lua REQUIRED)
find_package(sol2 CONFIG REQUIRED)


add_library(lua-base 
    src/lua_base.cpp
    src/lua_plugin.cpp
    
)
generate_export_header(lua-base EXPORT_FILE_NAME lua_base_export.hpp)
target_include_directories(lua-base PRIVATE 
    "${LUA_INCLUDE_DIR}"
    lua_protobuf
)
target_link_libraries(lua-base PRIVATE 
    sol2::sol2 
    spdlog::spdlog
    ${LUA_LIBRARIES}
)

target_include_directories(lua-base PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(lua-base PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)

file(COPY lua_protobuf/protoc.lua DESTINATION "${shared_output_dir}/lua/modules")
file(COPY my_plugins/ DESTINATION "${shared_output_dir}/lua/plugins")
install(FILES lua_protobuf/protoc.lua DESTINATION ${CMAKE_INSTALL_PLUGINDIR}/lua/modules)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libcon_export.hpp" TYPE INCLUDE)
install(TARGETS lua-base)

# add pb library to enable loading 
add_library(pb SHARED lua_protobuf/pb.c)
set_target_properties(pb PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)
target_compile_definitions(pb PRIVATE PB_IMPLEMENTATION)
target_link_libraries(pb PRIVATE ${LUA_LIBRARIES})
target_include_directories(pb PRIVATE "${LUA_INCLUDE_DIR}")
