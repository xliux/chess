#include <memory>

#include "chess/king.h"
#include "chess/board.h"
#include "gtest/gtest.h"
#include "chess/tests/util.h"

using namespace chess;
using namespace std;

TEST(King, canAttack) {
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::B_KING, 1, 3);
    auto blackKing = board->blackKing();
    EXPECT_FALSE(blackKing->canAttack(*board, Position(3, 5)));
    EXPECT_TRUE(blackKing->canAttack(*board, Position(2, 4)));
    EXPECT_TRUE(blackKing->canAttack(*board, Position(1, 2)));
    EXPECT_TRUE(blackKing->canAttack(*board, Position(0, 2)));
    EXPECT_FALSE(blackKing->canAttack(*board, Position(0, 1)));
    EXPECT_FALSE(blackKing->canAttack(*board, Position(-1, 1)));
  }
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::W_KING, 0, 1);
    auto whiteKing = board->whiteKing();
    EXPECT_TRUE(whiteKing->canAttack(*board, Position(0, 0)));
    EXPECT_TRUE(whiteKing->canAttack(*board, Position(1, 0)));
    EXPECT_TRUE(whiteKing->canAttack(*board, Position(1, 1)));
    EXPECT_FALSE(whiteKing->canAttack(*board, Position(2, 0)));
    EXPECT_FALSE(whiteKing->canAttack(*board, Position(0, 1)));
  }
}

TEST(King, getNextStates) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::B_KING, 0, 0);
  board->add(Piece::Type::W_ROOK, 1, 1);
  board->add(Piece::Type::W_KING, 5, 5);
  EXPECT_EQ(1, board->blackKing()->getNextStates(*board).size());

  board->add(Piece::Type::W_BISHOP, 2, 2);
  EXPECT_EQ(0, board->blackKing()->getNextStates(*board).size());
}
