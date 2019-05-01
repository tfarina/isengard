cmake_minimum_required(VERSION 2.6)
project(ini C)

set(SRCS
  ini.c
)

add_library(ini ${SRCS})
