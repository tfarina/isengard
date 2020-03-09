cmake_minimum_required(VERSION 2.6)
project(sab C)

add_compile_options(-std=c99)
add_compile_options(-pedantic)
add_compile_options(-pthread)
list(APPEND EXTRA_TARGET_LINK_LIBRARIES -pthread -ldl)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(third_party/sqlite)

set(LIB_SRCS
    src/ab.c
    src/alpm_list.c
    src/fort.c
    src/strutils.c
   )
add_library(ab ${LIB_SRCS})
target_link_libraries(ab sqlite3)

set(ADD_SRCS src/add.c)

add_executable(add ${ADD_SRCS})
target_link_libraries(add sqlite3 ab ${EXTRA_TARGET_LINK_LIBRARIES})

set(CHANGE_SRCS src/change.c)

add_executable(change ${CHANGE_SRCS})
target_link_libraries(change sqlite3 ab ${EXTRA_TARGET_LINK_LIBRARIES})

set(DELETE_SRCS src/delete.c)

add_executable(delete ${DELETE_SRCS})
target_link_libraries(delete sqlite3 ab ${EXTRA_TARGET_LINK_LIBRARIES})

set(SHOWREC_SRCS src/showrec.c)

add_executable(showrec ${SHOWREC_SRCS})
target_link_libraries(showrec sqlite3 ab ${EXTRA_TARGET_LINK_LIBRARIES})

add_compile_options(-std=c99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)

include_directories(${CMAKE_SOURCE_DIR})

set(SRCS
    src/cmd_add.c
    src/cmd_change.c
    src/cmd_delete.c
    src/cmd_list.c
    src/commands.h
    src/main.c
    )
add_executable(sab ${SRCS})
target_link_libraries(sab ab sqlite3 ${EXTRA_TARGET_LINK_LIBRARIES})

find_package(GTK2 2.6 REQUIRED gtk)

if (GTK2_FOUND)
  include_directories(${GTK2_INCLUDE_DIRS})

  add_executable(ui src/ui.c)
  target_link_libraries(ui sqlite3 ${GTK2_LIBRARIES} ${EXTRA_TARGET_LINK_LIBRARIES})

  add_executable(graph src/graph.c)
  target_link_libraries(graph -lm ${GTK2_LIBRARIES} ${EXTRA_TARGET_LINK_LIBRARIES})

  add_executable(sab_gui src/about_dialog.c src/sab_main_window.c)
  target_link_libraries(sab_gui ab sqlite3 ${GTK2_LIBRARIES} ${EXTRA_TARGET_LINK_LIBRARIES})
endif()
