#include "chess/piece.h"
#include "gtest/gtest.h"

using namespace chess;

TEST(Piece, isBlack) {
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_PAWN));
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_KING));
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_PAWN));
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_BISHOP));
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_KNIGHT));
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_ROOK));
  EXPECT_TRUE(Piece::isBlack(Piece::Type::B_QUEEN));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_PAWN));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_KING));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_PAWN));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_BISHOP));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_KNIGHT));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_ROOK));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::W_QUEEN));
  EXPECT_FALSE(Piece::isBlack(Piece::Type::EMPTY));
}
TEST(Piece, isWhite) {
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_PAWN));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_KING));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_PAWN));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_BISHOP));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_KNIGHT));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_ROOK));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::B_QUEEN));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_PAWN));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_KING));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_PAWN));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_BISHOP));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_KNIGHT));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_ROOK));
  EXPECT_TRUE(Piece::isWhite(Piece::Type::W_QUEEN));
  EXPECT_FALSE(Piece::isWhite(Piece::Type::EMPTY));
}

TEST(Piece, isSameColor) {
  EXPECT_TRUE(Piece::isSameColor(Piece::Type::B_PAWN, Piece::Type::B_KING));
  EXPECT_TRUE(Piece::isSameColor(Piece::Type::W_PAWN, Piece::Type::W_QUEEN));
  EXPECT_FALSE(Piece::isSameColor(Piece::Type::W_PAWN, Piece::Type::B_QUEEN));
  EXPECT_FALSE(Piece::isSameColor(Piece::Type::W_PAWN, Piece::Type::EMPTY));
  EXPECT_FALSE(Piece::isSameColor(Piece::Type::B_BISHOP, Piece::Type::EMPTY));
}

TEST(Piece, create) {
  Position pos(1, 2);
  EXPECT_EQ(pos, Piece::create(Piece::Type::B_KING, pos)->position());
  EXPECT_EQ(Piece::Type::B_KING, Piece::create(Piece::Type::B_KING, pos)->type());
}

TEST(Piece, setPosition) {
  Position pos1(5, 2);
  Position pos2(1, 1);
  EXPECT_EQ(pos2, Piece::create(Piece::Type::W_KING, pos1)->setPosition(pos2)->position());
}

