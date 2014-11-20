#include <memory>

#include <glog/logging.h>

#include "chess/bishop.h"
#include "chess/board.h"
#include "chess/bpawn.h"
#include "chess/king.h"
#include "chess/knight.h"
#include "chess/queen.h"
#include "chess/rook.h"
#include "chess/wpawn.h"
#include "gtest/gtest.h"
#include "chess/tests/util.h"

using namespace chess;
using namespace std;

TEST(Board, mateIn2) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::W_PAWN, 1, 0);
  board->add(Piece::Type::B_PAWN, 5, 0);

  board->add(Piece::Type::B_QUEEN, 1, 1);
  board->add(Piece::Type::W_BISHOP, 2, 1);
  board->add(Piece::Type::B_PAWN, 3, 1);

  board->add(Piece::Type::W_PAWN, 4, 2);
  board->add(Piece::Type::W_QUEEN, 6, 2);

  board->add(Piece::Type::W_ROOK, 0, 3);

  board->add(Piece::Type::B_KNIGHT, 1, 4);

  board->add(Piece::Type::W_KING, 0, 5);
  board->add(Piece::Type::W_PAWN, 1, 5);
  board->add(Piece::Type::B_PAWN, 6, 5);
  board->add(Piece::Type::B_ROOK, 7, 5);

  board->add(Piece::Type::W_PAWN, 1, 6);
  board->add(Piece::Type::B_PAWN, 6, 6);
  board->add(Piece::Type::B_KING, 7, 6);

  board->add(Piece::Type::W_PAWN, 2, 7);
  board->add(Piece::Type::B_PAWN, 6, 7);

  vector<Board::Layer> solutionTree;
#if TEST_BLACK_FIRST
  unique_ptr<Piece> bKing (new King(true, 7, 6));
  bool promo = false;
  auto newBoard = board->makeMove({6, 2}, {6, 5}, &promo);
  int score = newBoard->evaluate(true, 3, &solutionTree);
  EXPECT_EQ(-CHECKMATE, score);
#else
  int score = board->evaluate(false, 4, &solutionTree);
  EXPECT_EQ(CHECKMATE, score);
#endif
  printSolutionTree(solutionTree);
}

