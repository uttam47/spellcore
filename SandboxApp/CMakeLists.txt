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

# ----------------------------------------
# Post-build: Copy Resources folder
# ----------------------------------------
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

if(EXISTS ${RESOURCE_DIR})
    file(GLOB_RECURSE RESOURCE_FILES "${RESOURCE_DIR}/*")
    foreach(file ${RESOURCE_FILES})
        get_filename_component(dir ${file} DIRECTORY)
        file(RELATIVE_PATH rel_dir ${RESOURCE_DIR} ${dir})

        add_custom_command(
            TARGET SandboxApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SandboxApp>/Resources/${rel_dir}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${file}
                "$<TARGET_FILE_DIR:SandboxApp>/Resources/${rel_dir}"
        )
    endforeach()
endif()
