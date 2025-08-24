cmake_minimum_required(VERSION 3.15)
project(CommonUtility)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE CommonUtility_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
)

add_library(CommonUtility STATIC ${CommonUtility_SOURCES})

target_include_directories(CommonUtility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(CommonUtility PRIVATE Logger)
target_compile_definitions(CommonUtility PRIVATE LOG_TAG="CommonUtility")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CommonUtility_SOURCES})
