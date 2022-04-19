find_package(SDL2 CONFIG REQUIRED)

add_executable(device-tester 
    src/main.cpp
    src/icon.cpp
    src/application.hpp
    src/application.cpp
    src/application_vulkan.hpp
    src/application_vulkan.cpp
)
if(WIN32)
    if(MSVC)
        target_sources(device-tester PRIVATE WindowsHiDPI.manifest)
    elseif(MINGW)
        target_sources(device-tester PRIVATE WindowsHiDPI.rc)
    endif()
    target_link_libraries(device-tester PRIVATE dwmapi)
endif()

target_link_libraries(device-tester PRIVATE
    fmt::fmt
    SDL2::SDL2 
    SDL2::SDL2main
    imgui::imgui
    implot::implot
    gui-common
    gui-libcon
    gui-plugins
)
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in" 
    "${CMAKE_CURRENT_BINARY_DIR}/version.hpp"
)
target_include_directories(device-tester PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(device-tester PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${shared_output_dir}"
)

file(COPY resources DESTINATION "${shared_output_dir}")
install(DIRECTORY resources TYPE BIN)
install(TARGETS device-tester)
install(IMPORTED_RUNTIME_ARTIFACTS device-tester RUNTIME_DEPENDENCY_SET runtime-set)
install(RUNTIME_DEPENDENCY_SET runtime-set   
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
)
