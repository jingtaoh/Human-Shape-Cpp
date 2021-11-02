cmake_minimum_required(VERSION 3.19)
project(MoShape)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Eigen
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# spdlog
add_subdirectory(external/spdlog)

# matio
add_subdirectory(external/matio)

# glfw
add_subdirectory(external/glfw)

# gl3w
add_subdirectory(external/gl3w)

# glm
add_subdirectory(external/glm)

add_subdirectory(src)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})