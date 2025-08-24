cmake_minimum_required(VERSION 3.15)

project(SpellcoreRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather your own project source files
file(GLOB_RECURSE SpellcoreRenderer_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

# Combine everything
set(PROJECT_SOURCES
    ${SpellcoreRenderer_SOURCES}
)

# Create the static library
add_library(SpellcoreRenderer STATIC ${PROJECT_SOURCES})

# Include directories
target_include_directories(SpellcoreRenderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)
    
add_library(glew STATIC IMPORTED)
set_target_properties(glew PROPERTIES
    IMPORTED_LOCATION "${THIRD_PARTY_LIBS_DIR}/glew/lib/Release/x64/glew32s.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${THIRD_PARTY_LIBS_DIR}/glew/include"
)

target_link_libraries(SpellcoreRenderer PRIVATE Logger glew CommonUtility)

# Link OpenGL on Windows
if (WIN32)
    target_link_libraries(SpellcoreRenderer PRIVATE opengl32)
endif()

target_compile_definitions(SpellcoreRenderer PRIVATE GLEW_STATIC)
target_compile_definitions(SpellcoreRenderer PRIVATE LOG_TAG="SpellcoreRenderer")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES})
