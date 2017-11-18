cmake_minimum_required(VERSION 2.6)
project(network C)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c99 -pedantic -Wall -Wextra")

add_definitions(-D_GNU_SOURCE)

add_executable(hostname hostname.c)
add_executable(dnsclient dnsclient.c)
add_executable(dns_dname dns_dname.c)
add_executable(nameservers nameservers.c)
add_executable(selectserver selectserver.c)
add_executable(udpserver log.c udpserver.c)
add_executable(udpclient log.c udpclient.c)
add_executable(unixserver unixserver.c)
add_executable(unixclient unixclient.c)
