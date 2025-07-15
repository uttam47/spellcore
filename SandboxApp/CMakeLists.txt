cmake_minimum_required(VERSION 3.15)

project(SandboxApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

add_executable(SandboxApp ${PROJECT_SOURCES})

# Include directories
target_include_directories(SandboxApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${THIRD_PARTY_LIBS_DIR}/glm
)

# # Add the GLEW library
# add_library(glew STATIC IMPORTED)
# set_target_properties(glew PROPERTIES
#     IMPORTED_LOCATION "${THIRD_PARTY_LIBS_DIR}/glew/lib/Release/x64/glew32s.lib"
#     INTERFACE_INCLUDE_DIRECTORIES "${THIRD_PARTY_LIBS_DIR}/glew/include"
# )

# # Link GLEW to your app
# target_link_libraries(SandboxApp PRIVATE glew)


# Link dependencies
target_link_libraries(SandboxApp
    PRIVATE
        glfw
        SpellcoreRenderer
        RunesUI
        EventSystem
        Logger
        entt
)


target_compile_definitions(SandboxApp PRIVATE LOG_TAG="SandboxApp")