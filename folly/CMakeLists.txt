cmake_minimum_required(VERSION 2.6)
PROJECT(folly)

LINK_DIRECTORIES(${GLOG_DIR}/lib ${GFLAGS_DIR}/lib)

FILE(GLOB SRCS "*.cpp")

ADD_LIBRARY(folly ${SRCS})
TARGET_LINK_LIBRARIES(folly glog)
SET_TARGET_PROPERTIES(folly PROPERTIES LINK_FLAGS "-Wall -Wextra -Wl,--no-as-needed")
