#include <memory>

#include "chess/knight.h"
#include "chess/board.h"
#include "gtest/gtest.h"
#include "chess/tests/util.h"

using namespace chess;
using namespace std;

TEST(Knight, canAttack) {
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::W_KNIGHT, 1, 3);
    unique_ptr<Piece> whiteKnight(new Knight(false, 1, 3));

    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(2, 5)));
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(0, 1)));
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(0, 5)));
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(2, 1)));
    EXPECT_FALSE(whiteKnight->canAttack(*board, Position(1, 5)));
    EXPECT_FALSE(whiteKnight->canAttack(*board, Position(2, 4)));
    EXPECT_FALSE(whiteKnight->canAttack(*board, Position(-1, 1)));

    board->add(Piece::Type::B_BISHOP, 2, 3);
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(2, 5)));
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(0, 1)));
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(0, 5)));
    EXPECT_TRUE(whiteKnight->canAttack(*board, Position(2, 1)));
    EXPECT_FALSE(whiteKnight->canAttack(*board, Position(1, 5)));
    EXPECT_FALSE(whiteKnight->canAttack(*board, Position(2, 4)));
    EXPECT_FALSE(whiteKnight->canAttack(*board, Position(-1, 1)));
  }
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::W_KNIGHT, 2, 6);
    EXPECT_TRUE(board->piece(Position(2, 6))->canAttack(*board, {0, 7}));
  }
}

TEST(Knight, getNextStates) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::B_KING, 0, 0);
  board->add(Piece::Type::B_KNIGHT, 1, 1);
  board->add(Piece::Type::W_ROOK, 3, 3);
  board->add(Piece::Type::W_KING, 4, 3);
  unique_ptr<Piece> bKnight(new Knight(true, 1, 1));
  EXPECT_EQ(4, bKnight->getNextStates(*board).size());

  board->add(Piece::Type::B_PAWN, 0, 3);
  EXPECT_EQ(3, bKnight->getNextStates(*board).size());

  board->add(Piece::Type::W_BISHOP, 2, 2);
  EXPECT_EQ(0, bKnight->getNextStates(*board).size());
}
