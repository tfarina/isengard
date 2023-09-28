cmake_minimum_required(VERSION 2.6)
project(window_icon C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

find_package(GTK2 2.6 REQUIRED gtk)

if (GTK2_FOUND)
  add_executable(window_icon window_icon.c)
  target_include_directories(window_icon PUBLIC ${GTK2_INCLUDE_DIRS})
  target_link_libraries(window_icon ${GTK2_LIBRARIES})
endif()
