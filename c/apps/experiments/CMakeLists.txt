cmake_minimum_required(VERSION 2.8)
project(mysql C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")    
find_package(MySQL)

add_subdirectory(third_party/iniparser)

add_executable(showrec strutils.c db.c futils.c ffileutils.c showrec.c)
add_executable(add strutils.c db.c futils.c ffileutils.c add.c)
target_link_libraries(showrec ${MYSQL_LIBRARY} iniparser)
target_link_libraries(add ${MYSQL_LIBRARY} iniparser)
