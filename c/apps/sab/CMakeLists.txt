cmake_minimum_required(VERSION 2.6)
project(sab C)

add_compile_options(-std=c99)
add_compile_options(-pedantic)
add_compile_options(-pthread)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(third_party/sqlite)
add_subdirectory(src)

set(ADD_SRCS src/cli/add.c)

add_executable(add ${ADD_SRCS})
target_link_libraries(add ab)

set(CHANGE_SRCS src/cli/change.c)

add_executable(change ${CHANGE_SRCS})
target_link_libraries(change ab)

set(DELETE_SRCS src/cli/delete.c)

add_executable(delete ${DELETE_SRCS})
target_link_libraries(delete ab)

set(SHOWREC_SRCS src/cli/fort.c src/cli/common.c src/cli/showrec.c)

add_executable(showrec ${SHOWREC_SRCS})
target_link_libraries(showrec ab)

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

include_directories(${CMAKE_SOURCE_DIR})

set(SRCS
    src/cli/cmd_add.c
    src/cli/cmd_change.c
    src/cli/cmd_delete.c
    src/cli/cmd_list.c
    src/cli/commands.h
    src/cli/common.c
    src/cli/fort.c
    src/cli/main.c
    )
add_executable(sab ${SRCS})
target_link_libraries(sab ab)

add_subdirectory(src/gui)
add_subdirectory(example)
