cmake_minimum_required(VERSION 2.6)
project(gtk C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

find_package(GTK2 2.6 REQUIRED gtk)

if (GTK2_FOUND)
  add_subdirectory(third_party/sqlite)
  add_executable(ui ui.c)
  add_dependencies(ui sqlite3)
  target_include_directories(ui PUBLIC ${GTK2_INCLUDE_DIRS})
  target_link_libraries(ui sqlite3 ${GTK2_LIBRARIES})
endif()
