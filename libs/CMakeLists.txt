add_library(imgui-club INTERFACE)
target_include_directories(imgui-club INTERFACE imgui_club/imgui_memory_editor)

add_subdirectory(libcon)
add_subdirectory(plugin_base)
add_subdirectory(libplg)
add_subdirectory(gui-common)
add_subdirectory(gui-libcon)
add_subdirectory(gui-plugins)
