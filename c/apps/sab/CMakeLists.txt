cmake_minimum_required(VERSION 2.6)
project(sab C)

option(ENABLE_GTK "Build GTK+ client" OFF)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Place binaries for all build types in the same directory,
# so we know where to find it.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EXECUTABLE_OUTPUT_PATH})

add_subdirectory(third_party/sqlite)
add_subdirectory(src)

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

add_subdirectory(src/cli)

if (ENABLE_GTK)
  add_subdirectory(src/gui)
endif()
