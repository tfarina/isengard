add_subdirectory(polymorphism)
add_subdirectory(inheritance)
add_subdirectory(cli)
add_subdirectory(db)

add_executable(stringize_example stringize_example.c)
target_link_libraries(stringize_example PRIVATE common)

add_executable(arraysize_example arraysize_example.c)
target_link_libraries(arraysize_example PRIVATE common)

add_executable(buffer_example buffer_example.c)
target_link_libraries(buffer_example PRIVATE common)

add_executable(vector_example vector_example.c)
target_link_libraries(vector_example PRIVATE common)

add_executable(freenull freenull.c)
