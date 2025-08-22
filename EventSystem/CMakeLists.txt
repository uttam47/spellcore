cmake_minimum_required(VERSION 3.15)

project(EventSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather your own project source files
file(GLOB_RECURSE EventSystem_Sources CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

# Combine everything
set(PROJECT_SOURCES
    ${EventSystem_Sources}
)

# Create the static library
add_library(EventSystem STATIC ${PROJECT_SOURCES})

# Include directories
target_include_directories(EventSystem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/source/BlockingEvents
    ${CMAKE_CURRENT_SOURCE_DIR}/source/DefferedEvents
)
target_link_libraries(EventSystem
    PRIVATE
        Logger
)
target_compile_definitions(EventSystem PRIVATE LOG_TAG="EventManager")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES})