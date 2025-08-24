cmake_minimum_required(VERSION 3.15)
project(RunesUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Your sources
file(GLOB_RECURSE RUNESUI_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

# --- ImGui sources
set(IMGUI_DIR ${THIRD_PARTY_LIBS_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

set(PROJECT_SOURCES
    ${RUNESUI_SOURCES}
    ${IMGUI_SOURCES}
)

add_library(RunesUI STATIC ${PROJECT_SOURCES})

target_include_directories(RunesUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${IMGUI_DIR}
        ${IMGUI_BACKENDS_DIR}
    PRIVATE
        ${THIRD_PARTY_LIBS_DIR}/glfw/include
        ${THIRD_PARTY_LIBS_DIR}/glad/include
)

target_compile_definitions(RunesUI PRIVATE
    LOG_TAG="RunesUI"
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# --- Nice filters in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Your code appears directly under the project
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RUNESUI_SOURCES})
# Third-party ImGui grouped under a tidy folder
source_group(TREE ${IMGUI_DIR} PREFIX "ThirdParty/imgui" FILES ${IMGUI_SOURCES})
