#include <memory>
#include "chess/move.h"
#include "chess/one_round.h"
#include "gtest/gtest.h"
#include "glog/logging.h"

using namespace chess;
using namespace std;

class TestableOneRound : public OneRound {
  public: 
    explicit TestableOneRound(const Board* board, bool firstBlack=true): 
      OneRound(board, firstBlack, 0) {
        auxBoard_ = board->clone();
      }

    bool canMove(const Piece* piece, Position pos) {
      Move move(*auxBoard_, Move::NORMAL_MOVE, piece->position(), pos); 
      return OneRound::canMove(piece, move, false, auxBoard_.get());
    }
  private:
    unique_ptr<Board> auxBoard_;
};

class OneRoundTest : public ::testing::Test {
  public:
    virtual ~OneRoundTest() {}

  protected:
    virtual void SetUp() {
      board_.reset(new Board(false));
      board_->add(Piece::Type::W_KING, 0, 1);
      board_->add(Piece::Type::B_KING, 2, 1);
      board_->add(Piece::Type::B_ROOK, 2, 7);
      board_->add(Piece::Type::B_BISHOP, 4, 5);
      board_->add(Piece::Type::W_ROOK, 3, 4);
      oneRound_.reset(new TestableOneRound(board_.get()));
    }

    virtual void TearDown() {
      board_.reset();
      oneRound_.reset();
    }

    unique_ptr<Board> board_;
    unique_ptr<TestableOneRound> oneRound_;
};

TEST_F(OneRoundTest, canMove) {
  // can move king
  auto bKing = board_->blackKing();
  auto wKing = board_->whiteKing();
  EXPECT_TRUE(oneRound_->canMove(bKing.get(), Position(0, 1)));
  EXPECT_FALSE(oneRound_->canMove(bKing.get(), Position(1, 0)));
  EXPECT_FALSE(oneRound_->canMove(wKing.get(), Position(1, 0)));
  EXPECT_FALSE(oneRound_->canMove(wKing.get(), Position(1, 1)));
  EXPECT_TRUE(oneRound_->canMove(wKing.get(), Position(0, 2)));
  auto wRook = board_->piece(Position{3, 4});
  EXPECT_FALSE(oneRound_->canMove(wRook.get(), Position(3, 7)));
}

TEST(OneRound, canMove1) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::W_KING, 0, 0);
  board->add(Piece::Type::B_KING, 2, 7);
  board->add(Piece::Type::B_ROOK, 3, 6);
  board->add(Piece::Type::W_BISHOP, 4, 5);
  auto bRook = board->piece({3, 6});
  TestableOneRound oneRound(board.get());
  EXPECT_FALSE(oneRound.canMove(bRook.get(), Position(3, 7)));
}

TEST(OneRound, canMove2) {
  unique_ptr<Board> board(new Board(false));
  board->add(Piece::Type::W_KING, 5, 7);
  board->add(Piece::Type::B_KING, 7, 7);
  board->add(Piece::Type::B_KNIGHT, 6, 7);
  board->add(Piece::Type::W_PAWN, 5, 6);
  auto p = board->safePiece({5, 6});
  TestableOneRound oneRound(board.get(), false);
  EXPECT_TRUE(oneRound.canMove(p, Position(6, 7)));
  oneRound.expand(1);
}

TEST(OneRound, expand) {
  // White mate in 2 (board 37)
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

  // white first
  OneRound oneRound(board.get(), false, 0);
  int score = oneRound.expand(2);
  LOG(INFO) << "score = " << score << std::endl
    << OneRound::cache().stats() << std::endl
    << "path:" << endl << oneRound.printPath();
  
}
