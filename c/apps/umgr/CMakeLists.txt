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

set(SRCS src/db.c src/user.c src/list.c)

add_executable(list ${SRCS})
target_link_libraries(list sqlite3 ${EXTRA_TARGET_LINK_LIBRARIES})

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
