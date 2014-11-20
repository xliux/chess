cmake_minimum_required(VERSION 2.6)
project(GIT_WORK)

SET(GLOG_DIR /Users/xliu/homebrew/Cellar/glog/0.3.3)
SET(GFLAGS_DIR /Users/xliu/homebrew/Cellar/gflags/2.0)

INCLUDE_DIRECTORIES(${GIT_WORK_SOURCE_DIR} ${GLOG_DIR}/include ${GFLAGS_DIR}/include)
LINK_DIRECTORIES(${GLOG_DIR}/lib ${GFLAGS_DIR}/lib)

SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)
SET (CMAKE_EXPORT_COMPILE_COMMANDS "${CMAKE_EXPORT_COMPILE_COMMANDS}" CACHE STRING "export compile_commands.json")

add_subdirectory(third_party)
add_subdirectory(folly)
add_subdirectory(chess)
add_subdirectory(test)
