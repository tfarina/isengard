set(TARGETNAME sqlite)

set(SOURCES
  sqlite3.c
)

add_library(${TARGETNAME}
  ${SOURCES}
)

target_link_libraries(${TARGETNAME}
  -pthread
  ${CMAKE_DL_LIBS}
)
