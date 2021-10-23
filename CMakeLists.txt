cmake_minimum_required(VERSION 3.19)
project(MoShape)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Eigen
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# spdlog
add_subdirectory(external/spdlog)

add_subdirectory(src)