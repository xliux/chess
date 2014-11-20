#include <memory>

#include "chess/rook.h"
#include "chess/board.h"
#include "gtest/gtest.h"
#include "chess/tests/util.h"

using namespace chess;
using namespace std;

TEST(Rook, canAttack) {
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::W_ROOK, 1, 3);
    unique_ptr<Piece> wRook(new Rook(false, 1, 3));
    board->add(Piece::Type::B_BISHOP, 2, 2);

    EXPECT_TRUE(wRook->canAttack(*board, Position(1, 0)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(1, 2)));
    EXPECT_FALSE(wRook->canAttack(*board, Position(1, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(1, 4)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(1, 5)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(1, 6)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(1, 7)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(0, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(2, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(3, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(4, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(5, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(6, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(7, 3)));

    board->add(Piece::Type::W_PAWN, 2, 3);
    EXPECT_TRUE(wRook->canAttack(*board, Position(0, 3)));
    EXPECT_TRUE(wRook->canAttack(*board, Position(2, 3)));
    EXPECT_FALSE(wRook->canAttack(*board, Position(3, 3)));
    EXPECT_FALSE(wRook->canAttack(*board, Position(4, 3)));
    EXPECT_FALSE(wRook->canAttack(*board, Position(5, 3)));
    EXPECT_FALSE(wRook->canAttack(*board, Position(6, 3)));
    EXPECT_FALSE(wRook->canAttack(*board, Position(7, 3)));
  }
}

TEST(Rook, getNextStates) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::B_KING, 0, 0);
  board->add(Piece::Type::B_QUEEN, 1, 1);
  board->add(Piece::Type::W_ROOK, 3, 3);
  board->add(Piece::Type::W_KING, 4, 3);
  unique_ptr<Piece> wRook(new Rook(false, 3, 3));
  EXPECT_EQ(10, wRook->getNextStates(*board).size());

  board->add(Piece::Type::B_PAWN, 2, 2);
  EXPECT_EQ(10, wRook->getNextStates(*board).size());

  board->add(Piece::Type::B_ROOK, 1, 3);
  EXPECT_EQ(2, wRook->getNextStates(*board).size());
}
