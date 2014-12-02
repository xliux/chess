#include <glog/logging.h>
#include "chess/wpawn.h"

#include "chess/board.h"
#include "chess/move.h"
#include "chess/util.h"

namespace chess {
using namespace std;

void WPawn::addMove(
    const Board& board, Position newPos, vector<Move> *moves) const {
  DCHECK(moves != nullptr);
  if (newPos.first != 7) {
    moves->emplace_back(board, Move::NORMAL_MOVE, position(), newPos);
  } else {
    moves->emplace_back(board, Move::QUEEN_PROMOTION, position(), newPos);
    moves->emplace_back(board, Move::KNIGHT_PROMOTION, position(), newPos);
  }
}

vector<Move> WPawn::getMoves(const Board& board) const {
  vector<Move> moves;
  moves.reserve(8);

  Position newPos(row() + 1, col());
  if (canPlace(board, newPos) && board.isEmpty(newPos)) {
    addMove(board, newPos, &moves);
  }

  if (row() == 1 && board.isEmpty(newPos)) {
    newPos = Position(row() + 2, col());
    if (board.isEmpty(newPos) && canPlace(board, newPos)) {
      moves.emplace_back(board, Move::NORMAL_MOVE, position(), newPos);
    }
  }

  // capture left
  newPos = Position(row() + 1, col() - 1);
  if (!board.isEmpty(newPos) && canPlace(board, newPos)) {
    addMove(board, newPos, &moves);
  }
  // capture right
  newPos = Position(row() + 1, col() + 1);
  if (!board.isEmpty(newPos) && canPlace(board, newPos)) {
    addMove(board, newPos, &moves);
  }

  // en passant
  if (row() == 4 && board.fromType() == Type::B_PAWN
      && board.from().first == 6 && board.to().first == 4
      && abs<int>(board.from().second - col()) == 1) {
    moves.emplace_back(board, Move::PAWN_EN_PASSANT,
        position(), Position(row() + 1, board.from().second));
  }
  return moves;
}

bool WPawn::canAttack(const Board& board, Position pos) const {
  int8_t dc = col() - pos.second;
  return (pos.first == row() + 1)  && (dc == 1 || dc == -1);
}

}  // namespace chess
