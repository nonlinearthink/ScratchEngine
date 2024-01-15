cmake_minimum_required(VERSION 3.20)

message("Building with CMake version: ${CMAKE_VERSION}")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

project(ScratchEngine VERSION 0.1.0 LANGUAGES C CXX)

# Add defines
add_definitions(-DUNICODE -D_UNICODE)

# Add conan dependencies
find_package(imgui REQUIRED)

# Collect source files
set(
    SCRATCH_ENGINE_FILES
    src/main.cpp
    src/common.h
    src/engine.h
    src/engine.cpp
    src/d3d12_command.h
    src/d3d12_command.cpp
)
file(GLOB_RECURSE IMGUI_BINDINGS "${CMAKE_SOURCE_DIR}/bindings/*.h" "${CMAKE_SOURCE_DIR}/bindings/*.cpp")

# Build executable
add_executable(${PROJECT_NAME} ${SCRATCH_ENGINE_FILES} ${IMGUI_BINDINGS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/bindings)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

# include(CTest)
# enable_testing()
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
