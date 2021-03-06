project(device-tester)

set(LIB_NAME ${CMAKE_PROJECT_NAME})
set(APP_NAME ${CMAKE_PROJECT_NAME}_app)

find_package(implot CONFIG REQUIRED)
add_library(ImGui::ImGui ALIAS imgui::imgui)
find_package(Corrade REQUIRED Main PluginManager)
find_package(Magnum REQUIRED GL)
find_package(MagnumIntegration REQUIRED ImGui)
find_package(nlohmann_json CONFIG REQUIRED)

if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(CONNECTION_SRC 
    src/connection/config_io.cpp
    src/connection/connection.cpp
    src/connection/serial_connection.cpp
    src/connection/tcp_connection.cpp
    src/connection/udp_connection.cpp
    src/connection/tcp_server.cpp
    src/connection/tcp_server_client.cpp
    src/connection/tcp_server_manager.cpp
    src/connection/tcp_server_connection_handle.cpp
)

set(PROTOCOL_SRC 
    src/protocols/dynamic_protocol_dispatcher.cpp
    src/protocols/protocol_loader.cpp
)

set(DEVICE_SRC 
    src/devices/device_connection.cpp
    src/devices/base_device.cpp
)

set(CONFIG_SRC
    src/config/recent.cpp
)

set(GUI_SRC 
    src/gui/imgui_commons.cpp
    src/gui/device_manager.cpp
    src/gui/app.cpp
    src/gui/widgets/raw_text_output.cpp
    #src/gui/widgets/raw_mem_viewer.cpp
    src/gui/widgets/plot_throughput.cpp
    src/gui/widgets/inputs.cpp
    src/gui/widgets/connection_settings.cpp
    src/gui/modals/base_modal.cpp
    src/gui/modals/device_create.cpp
    src/gui/windows/base_window.cpp
    src/gui/windows/connection_win.cpp
    src/gui/windows/serial_connection_win.cpp
    src/gui/windows/tcp_connection_win.cpp
    src/gui/windows/udp_connection_win.cpp
    src/gui/windows/data_flow_editor.cpp
)

add_library(${LIB_NAME} STATIC
    ${CONNECTION_SRC}
    #${PROTOCOL_SRC}
    ${DEVICE_SRC}
    ${CONFIG_SRC}
    ${GUI_SRC}
)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
if(MSVC)
    set_property(TARGET ${LIB_NAME} APPEND PROPERTY COMPILE_OPTIONS "/bigobj")
endif()

set(COMPILE_DEFS BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
if(WIN32)
    # fix to win 10 yet
    list(APPEND COMPILE_DEFS _WIN32_WINNT=0x0A00)
endif()
target_compile_definitions(${LIB_NAME} PUBLIC ${COMPILE_DEFS})
target_link_libraries(${LIB_NAME} PUBLIC 
    Boost::headers
    Boost::graph
    spdlog::spdlog
    imgui::imgui
    implot::implot
    imgui::club
    dt::imnodes
    Corrade::Main
    Corrade::PluginManager
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    MagnumIntegration::ImGui
    nlohmann_json nlohmann_json::nlohmann_json
    dt::DtDataflowCore
    dt::DtDataflowEditor
)

add_executable(${APP_NAME} 
    src/main.cpp
)
set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${APP_NAME} PRIVATE imgui::imgui ${LIB_NAME})

# add hidpi instructions
if(WIN32)
    if(MSVC)
        target_sources(${APP_NAME} PRIVATE WindowsHiDPI.manifest)
    elseif(MINGW)
        target_sources(${APP_NAME} PRIVATE WindowsHiDPI.rc)
    endif()
endif()

add_custom_command(TARGET ${APP_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory fonts/ $<TARGET_FILE_DIR:${APP_NAME}>/fonts)

if(BUILD_TESTING)
    add_executable(${LIB_NAME}-tests 
        test/main.cpp
        test/protocols/protocol_dispatcher.cpp
    )
    set_property(TARGET ${LIB_NAME}-tests PROPERTY CXX_STANDARD 20)
    target_link_libraries(${LIB_NAME}-tests Catch2::Catch2 ${LIB_NAME})
    target_include_directories(${LIB_NAME}-tests PRIVATE src)

    include(Catch)
    catch_discover_tests(${LIB_NAME}-tests)
endif()

install(TARGETS ${APP_NAME})
