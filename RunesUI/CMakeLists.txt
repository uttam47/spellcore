cmake_minimum_required(VERSION 3.15)

project(RunesUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------------------
# Your sources
file(GLOB_RECURSE RUNESUI_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

# ImGui sources (explicit list is fine; keep it stable)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/ThirdPartyLibs/imgui)
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

# Combine everything
set(PROJECT_SOURCES
    ${RUNESUI_SOURCES}
    ${IMGUI_SOURCES}
)

# ---------------------------------------------------------------------------------------
# Library target
add_library(RunesUI STATIC ${PROJECT_SOURCES})

# Includes
target_include_directories(RunesUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${IMGUI_DIR}
        ${IMGUI_BACKENDS_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/ThirdPartyLibs/glfw/include
        ${CMAKE_SOURCE_DIR}/ThirdPartyLibs/glad/include
)

# If you use glad for OpenGL loader in ImGui backend, define this:
target_compile_definitions(RunesUI PRIVATE
    LOG_TAG="RunesUI"
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# ---------------------------------------------------------------------------------------
# Nice filters in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Root the tree at the repository top so both RunesUI/ and ThirdPartyLibs/ are under it
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${PROJECT_SOURCES})

# (Optional) Put the target itself under a folder in the VS Solution Explorer
set_target_properties(RunesUI PROPERTIES FOLDER "Engine/RunesUI")

# ---------------------------------------------------------------------------------------
# Linking:
# Keep GLFW/GLAD linked by the consumer (e.g., SandboxApp) if that's your plan.
# Otherwise, you could add:
# target_link_libraries(RunesUI PUBLIC glfw glad)
