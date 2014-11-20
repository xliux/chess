cmake_minimum_required(VERSION 2.6)
project(chess_tests)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${gtest_SOURCE_DIR}/include)
LINK_DIRECTORIES(${GLOG_DIR}/lib ${GFLAGS_DIR}/lib)

ADD_LIBRARY(test_util util.cc test_main.cc)
TARGET_LINK_LIBRARIES(test_util glog gtest gflags)

ADD_EXECUTABLE(piece_tests piece_test.cc)
TARGET_LINK_LIBRARIES(piece_tests glog test_util chess)

ADD_EXECUTABLE(king_tests king_test.cc)
TARGET_LINK_LIBRARIES(king_tests glog gtest test_util chess)

ADD_EXECUTABLE(bpawn_tests bpawn_test.cc)
TARGET_LINK_LIBRARIES(bpawn_tests glog gtest test_util chess)

ADD_EXECUTABLE(wpawn_tests wpawn_test.cc)
TARGET_LINK_LIBRARIES(wpawn_tests glog gtest test_util chess)

ADD_EXECUTABLE(knight_tests knight_test.cc)
TARGET_LINK_LIBRARIES(knight_tests glog gtest test_util chess)

ADD_EXECUTABLE(bishop_tests bishop_test.cc)
TARGET_LINK_LIBRARIES(bishop_tests glog gtest test_util chess)

ADD_EXECUTABLE(rook_tests rook_test.cc)
TARGET_LINK_LIBRARIES(rook_tests glog test_util chess)

ADD_EXECUTABLE(queen_tests queen_test.cc)
TARGET_LINK_LIBRARIES(queen_tests glog test_util chess)

ADD_EXECUTABLE(matein2_tests matein2_test.cc)
TARGET_LINK_LIBRARIES(matein2_tests glog test_util chess)

ADD_EXECUTABLE(one_round_tests one_round_test.cc)
TARGET_LINK_LIBRARIES(one_round_tests glog test_util chess)
