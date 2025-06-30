add_library(Logger STATIC
    source/LoggerManager.cpp
)

target_include_directories(Logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(Logger PUBLIC spdlog)
target_compile_definitions(Logger PRIVATE LOG_TAG="Logger")
