#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <memory>

namespace chess {
class Board;
std::unique_ptr<Board> createBoardForTest();

}
#endif
