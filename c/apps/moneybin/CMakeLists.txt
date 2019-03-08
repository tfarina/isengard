cmake_minimum_required(VERSION 2.8)
project(moneybin C)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

add_subdirectory(third_party/iniparser)
add_subdirectory(third_party/libcsv)
add_subdirectory(third_party/sqlite)

find_package(MySQL)

add_executable(print_csv_ticks print_csv_ticks.c strutils.c file.c csv_helper.c stock.c)
target_link_libraries(print_csv_ticks libcsv ${MYSQL_LIBRARY})

add_executable(print_csv_stats print_csv_stats.c strutils.c csv_helper.c stats.c)
target_link_libraries(print_csv_stats libcsv ${MYSQL_LIBRARY} -lm)

find_package(CURL REQUIRED)

if (CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})

  add_executable(gethistdata gethistdata.c ffileutils.c strutils.c futils.c db.c db_mysql.c stock.c file.c buffer.c curl_helper.c csv_helper.c)
  target_link_libraries(gethistdata ${CURL_LIBRARIES} libcsv iniparser ${MYSQL_LIBRARY})
endif()

add_executable(csvimport csvimport.c ffileutils.c strutils.c file.c futils.c db.c db_mysql.c stock.c csv_helper.c)
target_link_libraries(csvimport iniparser libcsv ${MYSQL_LIBRARY})
