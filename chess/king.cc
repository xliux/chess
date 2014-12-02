#include <stdlib.h>
#include <utility>
#include <vector>
#include <glog/logging.h>

#include "chess/piece.h"
#include "chess/board.h"
#include "chess/king.h"
#include "chess/move.h"
#include "chess/util.h"

DEFINE_bool(allow_castling, false, "allow castling");

namespace chess {
using namespace std;

vector<Move> King::getMoves(const Board& board) const {
  vector<Move> moves;
  moves.reserve(10);
  static DeltaMove kDelta[] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

  for (int i = 0; i < array_size(kDelta); ++i) {
    Position newPos(row() + kDelta[i].dr, col() + kDelta[i].dc);
    if (canPlace(board, newPos)) {
      moves.emplace_back(board, Move::NORMAL_MOVE, position(), newPos);
    }
  }
  if (FLAGS_allow_castling) {
    if (canCastle(board, true)) {
      moves.emplace_back(board, 
          Move::KING_CASTLING, position(), Position(row(), col() - 2));
    } 
    if (canCastle(board, false)) {
      moves.emplace_back(board,
          Move::KING_CASTLING, position(), Position(row(), col() + 2));
    }
  }
  return moves;
}

bool King::canCastle(const Board& board, bool smaller) const {
  if (position() != origPos()) return false; 
  int8_t rookDelta = smaller ? -4 : 3;
  Piece::Type rType = board.getPieceTypeAtPosition({row(), col() + rookDelta});
  if (rType != Piece::Type::B_ROOK && rType != Piece::Type::B_ROOK
      && isSameColor(rType, type())) {
    return false;
  }
  if (smaller) {
    for (int8_t i = 0; i < 4; ++i) {
      if (!board.isEmpty({row(), col() - i})) return false;
    }
  } else {
    for (int8_t i = 0; i < 3; ++i) {
      if (!board.isEmpty({row(), col() + i})) return false;
    }
  }
  return true;
}

bool King::canAttack(const Board& board, Position pos) const {
  if (pos == position()) return false;
  int8_t dr = pos.first - row();
  int8_t dc = pos.second - col();
  return (dr <= 1 && dr >= -1) && (dc <= 1 && dc >= -1);
}

}  // namespace chess
