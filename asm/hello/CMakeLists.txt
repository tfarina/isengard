cmake_minimum_required(VERSION 2.8)
project(hello ASM)

include(CheckLanguage)

check_language(ASM)

if(CMAKE_ASM_COMPILER)
  enable_language(ASM)
endif()

if(MSVC)
  enable_language(ASM_MASM)
else()
  check_language(ASM-ATT)
  if(CMAKE_ASM-ATT_COMPILER)
    enable_language(ASM-ATT)
  endif()
endif()

set(SRCS
  hello_x86.s
)

add_executable(hello ${SRCS})

set(SRCS_x64
  hello_x64.s
)

add_executable(hello_x64 ${SRCS_x64})
