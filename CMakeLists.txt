cmake_minimum_required(VERSION 3.15)
file(READ vcpkg.json VCPKG_MANIFEST_STR)
string(JSON x_proj_ver GET ${VCPKG_MANIFEST_STR} "version-semver")
project(device-tester VERSION ${x_proj_ver} LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_REF)
message(STATUS "GIT_REF: ${GIT_REF}")
message(STATUS "GIT_REFSPEC: ${GIT_REFSPEC}")

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "build libraries as shared" ON)

find_package(Boost REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

set(shared_output_dir "${CMAKE_CURRENT_BINARY_DIR}/out")
set(CMAKE_INSTALL_PLUGINDIR ${CMAKE_INSTALL_BINDIR}/plugins)

include(GenerateExportHeader)
add_subdirectory(libs)
add_subdirectory(plugins)
add_subdirectory(app)
