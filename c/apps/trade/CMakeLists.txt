cmake_minimum_required(VERSION 2.8)
project(trade C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIBCSV_SRCS
    libcsv.c
    )

add_library(libcsv ${LIBCSV_SRCS})

set(INIPARSER_SRCS
    dictionary.c
    iniparser.c
    )

add_library(iniparser ${INIPARSER_SRCS})

add_executable(ticks ticks.c file.c)
target_link_libraries(ticks libcsv)

find_package(CURL REQUIRED)
if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
  add_executable(gethistdata gethistdata.c db_mysql.c)
  target_link_libraries(gethistdata ${CURL_LIBRARIES} libcsv iniparser ${MYSQL_LIBRARY})
endif()

add_executable(csvimport csvimport.c file.c db_mysql.c)
target_link_libraries(csvimport iniparser libcsv ${MYSQL_LIBRARY})
