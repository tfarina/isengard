cmake_minimum_required(VERSION 2.6)
project(polymorphism C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

set(SRCS
  animal.c
  duck.c
)

add_library(libpm ${SRCS})

add_executable(animal_test animal_test.c)
target_link_libraries(animal_test libpm)


