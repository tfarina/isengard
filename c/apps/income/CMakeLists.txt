cmake_minimum_required(VERSION 2.8)
project(trade C)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

set(LIBCSV_SRCS
    libcsv.c
    )

add_library(libcsv ${LIBCSV_SRCS})

set(INIPARSER_SRCS
    dictionary.c
    iniparser.c
    )

add_library(iniparser ${INIPARSER_SRCS})

find_package(MySQL)

add_executable(print_csv_ticks print_csv_ticks.c file.c csv_helper.c stock.c)
target_link_libraries(print_csv_ticks libcsv ${MYSQL_LIBRARY})

find_package(CURL REQUIRED)
if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
  add_executable(gethistdata gethistdata.c env.c db_mysql.c stock.c file.c buffer.c curl_helper.c csv_helper.c)
  target_link_libraries(gethistdata ${CURL_LIBRARIES} libcsv iniparser ${MYSQL_LIBRARY})
endif()

add_executable(csvimport csvimport.c file.c db_mysql.c env.c stock.c csv_helper.c)
target_link_libraries(csvimport iniparser libcsv ${MYSQL_LIBRARY})
