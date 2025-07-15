cmake_minimum_required(VERSION 3.15)

project(Spellcore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add external libraries (GLAD, GLFW, ImGui) 
add_subdirectory(ThirdPartyLibs)

#Set variables
set(THIRD_PARTY_LIBS_DIR ${CMAKE_SOURCE_DIR}/ThirdPartyLibs)

# Add subprojects
add_subdirectory(EventSystem)
add_subdirectory(SpellcoreRenderer)
add_subdirectory(RunesUI)
add_subdirectory(SandboxApp)
add_subdirectory(ThirdPartyLibs/spdlog)
add_subdirectory(Logger)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ThirdPartyLibs/entt/src)