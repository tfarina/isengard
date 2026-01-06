set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra -std=c99 -pedantic")

set(SRCS
  animal.c
  duck.c
)

add_library(libpm ${SRCS})

add_executable(polymorphism_example polymorphism_example.c)
target_link_libraries(polymorphism_example libpm)
