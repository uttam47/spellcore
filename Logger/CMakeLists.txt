cmake_minimum_required(VERSION 3.15)
project(Logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE Logger_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

add_library(Logger STATIC ${Logger_SOURCES})

target_include_directories(Logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(Logger PUBLIC spdlog)
target_compile_definitions(Logger PRIVATE LOG_TAG="Logger")

# Consistent VS filters like your other projects
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Logger_SOURCES})
