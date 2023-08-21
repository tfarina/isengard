cmake_minimum_required(VERSION 2.6)
project(frame C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

find_package(GTK2 2.6 REQUIRED gtk)

if (GTK2_FOUND)
  include_directories(${GTK2_INCLUDE_DIRS})

  add_executable(frames frames.c)
  target_link_libraries(frames ${GTK2_LIBRARIES})
endif()
