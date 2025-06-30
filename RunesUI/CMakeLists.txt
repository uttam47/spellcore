cmake_minimum_required(VERSION 3.15)

project(RunesUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather your own source files
file(GLOB_RECURSE RunesUI_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

# Gather ImGui source files
file(GLOB IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/backends/imgui_impl_opengl3.cpp
)

# Combine everything
set(PROJECT_SOURCES
    ${RunesUI_SOURCES}
    ${IMGUI_SOURCES}
)

# Create the library
add_library(RunesUI STATIC ${PROJECT_SOURCES})

# Include directories
target_include_directories(RunesUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdPartyLibs/imgui/backends
)

target_compile_definitions(RunesUI PRIVATE LOG_TAG="RunesUI")

# No need for explicit linking yet (GLFW is linked separately by the SandboxApp)
