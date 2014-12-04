cmake_minimum_required(VERSION 2.6)
PROJECT(chess)

LINK_DIRECTORIES(${GLOG_DIR}/lib ${GFLAGS_DIR}/lib)

FILE(GLOB SRCS "*.cc")

ADD_LIBRARY(chess ${SRCS})
TARGET_LINK_LIBRARIES(chess glog folly)

ADD_EXECUTABLE(checkmate checkmate.cc)
TARGET_LINK_LIBRARIES(checkmate chess glog gflags)

ADD_EXECUTABLE(dfs dfs.cc)
TARGET_LINK_LIBRARIES(dfs chess glog gflags tcmalloc profiler folly)
SET_TARGET_PROPERTIES(dfs PROPERTIES LINK_FLAGS "-Wall -Wextra -Wl,--no-as-needed")

ADD_SUBDIRECTORY(tests)
