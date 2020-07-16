cmake_minimum_required(VERSION 2.6)
project(apps C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

add_definitions(-D_GNU_SOURCE)

# Add local cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

set(MATH_LIB "-lm")

add_subdirectory(third_party/iniparser)
add_subdirectory(third_party/libcsv)
add_subdirectory(third_party/cJSON)
add_subdirectory(third_party/mxml)

add_executable(tst tst.c)
target_link_libraries(tst libcsv)

add_executable(ticks ticks.c)
target_link_libraries(ticks libcsv)

add_executable(readfile ffileutils.c readfile.c)
add_executable(writefile ffileutils.c writefile.c)
add_executable(buildtime buildtime.c)
add_executable(linebyline linebyline.c)
add_executable(stream_test stream.c stream_test.c)
add_executable(math_util_test math_util.c math_util_test.c)
target_link_libraries(math_util_test ${MATH_LIB})
add_executable(differentiation differentiation.c)
target_link_libraries(differentiation ${MATH_LIB})
add_executable(power2 power2.c)
add_executable(cansse2 cpuinfo.c cansse2.c)
add_executable(lexer compiler/ffileutils.c compiler/lexer.c)
add_executable(logging lookup.c logging.c)
add_executable(lookup_test lookup.c lookup_test.c)
add_executable(execprog execprog.c)
add_executable(getcwd getcwd.c)
add_executable(exampled exampled.c)
add_executable(strdup strutils.c strdup.c)
add_executable(ascii_table ascii_table.c)
add_executable(bit bit.c)
add_executable(homedir futils.c homedir.c)
add_executable(username futils.c username.c)
add_executable(isdir test.c isdir.c)
add_executable(configdir ffileutils.c futils.c configdir.c)
add_executable(cpucount cpuinfo.c cpucount.c)
add_executable(ascii ascii.c)
add_executable(pointers pointers.c)
add_executable(pointers2 pointers2.c)
add_executable(pointers3 pointers3.c)
add_executable(arrays_n_pointers arrays_n_pointers.c)
add_executable(postfixoperator postfixoperator.c)
add_executable(inheritance inheritance.c)
add_executable(arraysize arraysize.c)
add_executable(readjson ffileutils.c readjson.c)
target_link_libraries(readjson cjson)
add_executable(readmenujson ffileutils.c readmenujson.c)
target_link_libraries(readmenujson cjson)
add_executable(select select.c)
add_executable(percentage percentage.c)
target_link_libraries(percentage ${MATH_LIB})
add_executable(growth_rate financial.c growth_rate.c)
target_link_libraries(growth_rate ${MATH_LIB})
add_executable(future_value financial.c future_value.c)
target_link_libraries(future_value ${MATH_LIB})
add_executable(present_value financial.c present_value.c)
target_link_libraries(present_value ${MATH_LIB})
add_executable(pivot_test pivot.c pivot_test.c)
add_executable(querydate querydate.c)
add_executable(sizeof sizeof.c)
add_executable(cprimitivedatatypes cprimitivedatatypes.c)
add_executable(ccustomdatatypes ccustomdatatypes.c)
add_executable(ctypequalifiers ctypequalifiers.c)
add_executable(cstorageclassspecifiers cstorageclassspecifiers.c)
add_executable(ckeywords ckeywords.c)
add_executable(stats_test stats_test.c stats.c)
target_link_libraries(stats_test ${MATH_LIB})
add_executable(ansi_colors_test ansi_colors_test.c)
add_executable(pidfile pidfile.c)
add_executable(buildpath ffileutils.c buildpath.c)

set(LIBCSV_PARSER_SRCS
  csv_parser.c
)

add_library(libcsv_parser ${LIBCSV_PARSER_SRCS})
add_executable(csvimport csvimport.c)
target_link_libraries(csvimport libcsv_parser)

find_package(GTK2 2.6)

if (GTK2_FOUND)
  add_executable(graph graph.c)

  target_include_directories(graph PUBLIC ${GTK2_INCLUDE_DIRS})

  target_link_libraries(graph ${MATH_LIB} ${GTK2_LIBRARIES})
endif()

# These options are settable from the CMake command line. For example, to disable
# SQL, put -D WITH_SQL=OFF on the command line.

option(WITH_SQL "Build this project with SQL (mysql) support" OFF)

if (WITH_SQL)
  find_package(MySQL)
  add_executable(mysql_version mysql_version.c)
  target_link_libraries(mysql_version ${MYSQL_LIBRARY})
endif()

add_executable(writexml writexml.c)
target_link_libraries(writexml mxml -pthread)

add_executable(readxml readxml.c)
target_link_libraries(readxml mxml -pthread)
