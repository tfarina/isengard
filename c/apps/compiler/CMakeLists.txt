cmake_minimum_required(VERSION 2.6)
project(compiler C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c89 -pedantic")

add_definitions(-D_GNU_SOURCE)

add_executable(lex ffileutils.c lexer.c main.c)
