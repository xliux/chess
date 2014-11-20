#include <memory>

#include "chess/bishop.h"
#include "chess/board.h"
#include "gtest/gtest.h"
#include "chess/tests/util.h"

using namespace chess;
using namespace std;

TEST(Bishop, canAttack) {
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::W_BISHOP, 1, 3);
    unique_ptr<Piece> wBishop(new Bishop(false, 1, 3));

    EXPECT_TRUE(wBishop->canAttack(*board, Position(2, 4)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(3, 5)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(4, 6)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(5, 7)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(0, 2)));
    EXPECT_FALSE(wBishop->canAttack(*board, Position(1, 2)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(0, 4)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(2, 2)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(3, 1)));
    EXPECT_TRUE(wBishop->canAttack(*board, Position(4, 0)));

    board->add(Piece::Type::B_BISHOP, 3, 1);
    EXPECT_TRUE(wBishop->canAttack(*board, Position(3, 1)));
    EXPECT_FALSE(wBishop->canAttack(*board, Position(4, 0)));

    board->add(Piece::Type::B_PAWN, 2, 4);
    EXPECT_TRUE(wBishop->canAttack(*board, Position(2, 4)));
    EXPECT_FALSE(wBishop->canAttack(*board, Position(3, 5)));
    EXPECT_FALSE(wBishop->canAttack(*board, Position(4, 6)));
    EXPECT_FALSE(wBishop->canAttack(*board, Position(5, 7)));
  }
}

TEST(Bishop, getNextStates) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::B_KING, 0, 0);
  board->add(Piece::Type::B_BISHOP, 1, 1);
  board->add(Piece::Type::W_BISHOP, 3, 3);
  board->add(Piece::Type::W_KING, 4, 3);
  unique_ptr<Piece> bBishop(new Bishop(true, 1, 1));
  EXPECT_EQ(2, bBishop->getNextStates(*board).size());

  board->add(Piece::Type::B_PAWN, 2, 2);
  EXPECT_EQ(2, bBishop->getNextStates(*board).size());

  board->add(Piece::Type::B_ROOK, 0, 2);
  EXPECT_EQ(1, bBishop->getNextStates(*board).size());
}
