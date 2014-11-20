#include <memory>

#include "chess/bpawn.h"
#include "chess/board.h"
#include "gtest/gtest.h"
#include "chess/tests/util.h"

using namespace chess;
using namespace std;

TEST(BPawn, canAttack) {
  {
    unique_ptr<Board> board(new Board(false));
    board->add(Piece::Type::B_PAWN, 1, 3);
    auto_ptr<Piece> blackPawn(new BPawn(1, 3));
    EXPECT_TRUE(blackPawn->canAttack(*board, Position(0, 2)));
    EXPECT_TRUE(blackPawn->canAttack(*board, Position(0, 4)));
    EXPECT_FALSE(blackPawn->canAttack(*board, Position(1, 2)));
    EXPECT_FALSE(blackPawn->canAttack(*board, Position(1, 3)));
    EXPECT_FALSE(blackPawn->canAttack(*board, Position(1, 4)));
    EXPECT_FALSE(blackPawn->canAttack(*board, Position(2, 1)));
    EXPECT_FALSE(blackPawn->canAttack(*board, Position(2, 2)));
    EXPECT_FALSE(blackPawn->canAttack(*board, Position(2, 3)));
  }
}

TEST(BPawn, getNextStates) {
}
