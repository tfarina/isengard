cmake_minimum_required(VERSION 2.6)
project(imap C)

find_package(CURL REQUIRED)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall -Wextra")

add_definitions(-D_GNU_SOURCE)

include_directories(${CURL_INCLUDE_DIR})

add_executable(imap imap.c)
target_link_libraries(imap -lcurl)

