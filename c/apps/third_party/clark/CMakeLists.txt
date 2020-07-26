cmake_minimum_required(VERSION 2.6)
project(chashtable C)

set(SRCS
  hashtable.c
)

add_library(chashtable ${SRCS})
