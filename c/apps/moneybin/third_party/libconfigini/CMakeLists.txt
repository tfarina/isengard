cmake_minimum_required(VERSION 2.6)
project(libconfigini C)

set(SRCS
  configini.c
)

add_library(libconfigini ${SRCS})
