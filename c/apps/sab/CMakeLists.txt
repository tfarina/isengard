cmake_minimum_required(VERSION 2.6)
project(sab C)

option(ENABLE_GTK "Build GTK+ client" OFF)

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
  add_subdirectory(src/examples)
endif()
