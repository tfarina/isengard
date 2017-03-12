cmake_minimum_required(VERSION 2.6)
project(cli C)

add_compile_options(-std=c99)
add_compile_options(-pthread)
list(APPEND EXTRA_TARGET_LINK_LIBRARIES -pthread -ldl)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(third_party/sqlite)

set(SRCS cli/db.c cli/user.c cli/list.c)

add_executable(list ${SRCS})
target_link_libraries(list sqlite3 ${EXTRA_TARGET_LINK_LIBRARIES})
