cmake_minimum_required(VERSION 2.6)
project(test)

add_executable(test hello.cc)
target_link_libraries(test glog)
