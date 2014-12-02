#include <string>
#include <glog/logging.h>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/piece.h"
#include "chess/piece_bank.h"

namespace chess {
using namespace std;

Move::Move(const Board& board, SpecialType special, Position from, Position to) 
  : special_(special), from_(from), to_(to), 
  fromType_(board.getPieceTypeAtPosition(from)),
  toType_(board.getPieceTypeAtPosition(to)) {}

void Move::apply(Board* board) const {
  CHECK_NOTNULL(board);
  board->set(Piece::Type::EMPTY, from_);
  switch (special_) {
    case QUEEN_PROMOTION:
      board->set(Piece::getSameQueenType(fromType_), to_);
      break;
    case KNIGHT_PROMOTION:
      board->set(Piece::getSameKnightType(fromType_), to_);
      break;
    case PAWN_EN_PASSANT:
      setupEnPassant(board);
      board->set(fromType_, to_);
      break;
    case KING_CASTLING:
      setupCastling(board);
      board->set(fromType_, to_);
      break;
    default:
      CHECK(fromType_ != Piece::Type::EMPTY);
      board->set(fromType_, to_);
  }
}

void Move::undo(Board* board) const {
  board->set(toType_, to_);
  board->set(fromType_, from_);
  if (special_ == PAWN_EN_PASSANT) {
    undoEnPassant(board);
  } else if (special_ == KING_CASTLING) {
    undoCastling(board);
  }
}

void Move::setupEnPassant(Board* board) const {
  CHECK_NOTNULL(board);
  Position enPassantPos = to_;
  if (fromType_ == Piece::Type::B_PAWN) {
    ++enPassantPos.first;
    CHECK(board->getPieceTypeAtPosition(enPassantPos) ==
        Piece::Type::W_PAWN) << enPassantPos;
  } else {
    CHECK(fromType_ == Piece::Type::W_PAWN);
    --enPassantPos.first;
    CHECK(board->getPieceTypeAtPosition(enPassantPos) ==
        Piece::Type::B_PAWN) << enPassantPos;
  }
  board->set(Piece::Type::EMPTY, enPassantPos);
}

void Move::undoEnPassant(Board* board) const {
  DCHECK(board != nullptr);
  if (fromType_ == Piece::Type::B_PAWN) {
    board->set(Piece::Type::W_PAWN, {to_.first + 1, to_.second});
  } else {
    board->set(Piece::Type::B_PAWN, {to_.first - 1, to_.second});
  }
}

void Move::setupCastling(Board* board) const {
  DCHECK(board != nullptr);
  auto rPos = getCastlingRookPos();
  auto rType = board->getPieceTypeAtPosition(rPos.first);
  board->set(Piece::Type::EMPTY, rPos.first);
  board->set(rType, rPos.second);
}

void Move::undoCastling(Board* board) const {
  DCHECK(board != nullptr);
  auto rPos = getCastlingRookPos();
  auto rType = board->getPieceTypeAtPosition(rPos.second);
  board->set(Piece::Type::EMPTY, rPos.second);
  board->set(rType, rPos.first);
}

string Move::print() const {
  string output;
  output.reserve(10);
  output.append(PieceBank::get(fromType(), from())->print());
  if (special_ == QUEEN_PROMOTION || special_ == KNIGHT_PROMOTION) {
    output.append(" x ");
  } else {
    output.append(" - ");
  }
  output.append(PieceBank::get(newType(), to_)->print());
  return output;
}

string Move::notation() const {
  string notation = " ";
  switch (special()) {
    case PAWN_EN_PASSANT: notation = "^"; break;
    case KING_CASTLING:   notation = "0"; break;
    case QUEEN_PROMOTION: notation = "x"; break;
    case KNIGHT_PROMOTION: notation = "x"; break;
    default: notation = " ";
  }
  return notation + PieceBank::get(newType(), to_)->print();
}

float Move::heuristicScore(const Board& board) const {
  return PieceBank::get(newType(), to_)->heuristicScore(board);
}

pair<Position,Position> Move::getCastlingRookPos() const {
  CHECK(special_ == KING_CASTLING);
  int8_t preMoveCol, postMoveCol;
  if (to().second == 2) {
    preMoveCol = 0; 
    postMoveCol = 3;
  } else {
    DCHECK(to().second == 6);
    preMoveCol = 7; 
    postMoveCol = 5;
  }
  return pair<Position, Position>(
      {from().first, preMoveCol}, {from().first, postMoveCol});
}

}
