cmake_minimum_required(VERSION 2.6)
project(uzrm C)

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

include_directories(${CMAKE_SOURCE_DIR})

set(SRCS commands.h cmd_add.c cmd_list.c cmd_del.c cmd_update main.c)

add_executable(uzrm ${SRCS})
