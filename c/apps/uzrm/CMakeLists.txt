cmake_minimum_required(VERSION 2.6)
project(uzrm C)

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

include_directories(${CMAKE_SOURCE_DIR})

set(LIB_SRCS
    cmd_add.c
    cmd_del.c
    cmd_list.c
    cmd_update.c
    commands.h
    )
add_library(uzr ${LIB_SRCS})

set(EXE_SRCS main.c)
add_executable(uzrm ${EXE_SRCS})
target_link_libraries(uzrm uzr)
