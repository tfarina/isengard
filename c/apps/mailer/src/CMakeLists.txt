find_package(GTK2 2.6 REQUIRED gtk)

#set(DEPENDENCIES
#  smtp-client
#)

set(SOURCES
  compose.c
  main.c
)

add_executable(mailer
  ${SOURCES}
)

#add_dependencies(mailer
#  ${DEPENDENCIES}
#)

target_include_directories(mailer PUBLIC
  ${GTK2_INCLUDE_DIRS}
)

target_link_libraries(mailer
  ${DEPENDENCIES}
  ${GTK2_LIBRARIES}
)
