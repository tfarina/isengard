cmake_minimum_required(VERSION 2.8)
project(mysql C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")    
find_package(MySQL)

add_subdirectory(third_party/iniparser)

add_executable(list futils.c ffileutils.c list.c)
add_executable(insert futils.c ffileutils.c insert.c)
target_link_libraries(list ${MYSQL_LIBRARY} iniparser)
target_link_libraries(insert ${MYSQL_LIBRARY} iniparser)
