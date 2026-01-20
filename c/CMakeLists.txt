cmake_minimum_required(VERSION 2.6)
project(CPlayground C)

# Add local cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/examples/cmake/modules")

add_subdirectory(common)
add_subdirectory(examples)
add_subdirectory(experiments)
add_subdirectory(demos)
