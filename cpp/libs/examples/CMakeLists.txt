add_executable(absolutepath absolutepath.cc)
target_include_directories(absolutepath PUBLIC ..)
target_link_libraries(absolutepath base)

add_executable(pathexists pathexists.cc)
target_include_directories(pathexists PUBLIC ..)
target_link_libraries(pathexists base)
