cmake_minimum_required(VERSION 3.20)

project(ScratchEngine VERSION 0.1.0 LANGUAGES C CXX)

find_package(imgui REQUIRED)

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# include(CTest)
# enable_testing()
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)