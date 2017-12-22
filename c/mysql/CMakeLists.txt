cmake_minimum_required(VERSION 2.8)
project(mysql C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")    
find_package(MySQL)

add_executable(list_bookmarks list_bookmarks.c)
add_executable(insert_bookmark insert_bookmark.c)
target_link_libraries(list_bookmarks ${MYSQL_LIBRARY})
target_link_libraries(insert_bookmark ${MYSQL_LIBRARY})
