cmake_minimum_required(VERSION 2.6)
project(umgr C)

add_compile_options(-std=c99)
add_compile_options(-pthread)
list(APPEND EXTRA_TARGET_LINK_LIBRARIES -pthread -ldl)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(third_party/sqlite)

set(SRCS src/db.c src/user.c src/list.c)

add_executable(list ${SRCS})
target_link_libraries(list sqlite3 ${EXTRA_TARGET_LINK_LIBRARIES})
