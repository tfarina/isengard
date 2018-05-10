cmake_minimum_required(VERSION 2.6)
project(umgr C)

add_compile_options(-std=c99)
add_compile_options(-pthread)
list(APPEND EXTRA_TARGET_LINK_LIBRARIES -pthread -ldl)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(third_party/sqlite)

set(LIB_SRCS
    src/db.c
    src/user.c
   )
add_library(ulib ${LIB_SRCS})
target_link_libraries(ulib sqlite3)

set(INSERT_SRCS src/insert.c)

add_executable(insert ${INSERT_SRCS})
target_link_libraries(insert sqlite3 ulib ${EXTRA_TARGET_LINK_LIBRARIES})


set(LIST_SRCS src/list.c)

add_executable(list ${LIST_SRCS})
target_link_libraries(list sqlite3 ulib ${EXTRA_TARGET_LINK_LIBRARIES})

set(UPDATE_SRCS src/update.c)

add_executable(update ${UPDATE_SRCS})
target_link_libraries(update sqlite3 ulib ${EXTRA_TARGET_LINK_LIBRARIES})

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

include_directories(${CMAKE_SOURCE_DIR})

set(LIB_SRCS
    src/cmd_add.c
    src/cmd_del.c
    src/cmd_list.c
    src/cmd_update.c
    src/commands.h
    )
add_library(umg ${LIB_SRCS})

set(EXE_SRCS src/main.c)
add_executable(umgr ${EXE_SRCS})
target_link_libraries(umgr umg)
