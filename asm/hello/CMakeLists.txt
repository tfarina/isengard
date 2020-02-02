cmake_minimum_required(VERSION 2.8)
project(hello ASM)

set(SRCS
  i386/hello.s
)

add_executable(hello ${SRCS})
