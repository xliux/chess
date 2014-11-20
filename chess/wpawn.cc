#include <glog/logging.h>

#include "wpawn.h"
#include "board.h"
#include "util.h"

namespace chess {
using namespace std;

vector<Position> WPawn::getMoves(const Board& board) const {
  vector<Position> moves;

  Position newPos(row() + 1, col());
  if (canPlace(board, newPos) && board.isEmpty(newPos)) {
    moves.emplace_back(newPos);
  }
  if (row() == 1 && board.isEmpty(newPos)) {
    newPos = Position(row() + 2, col());
    if (board.isEmpty(newPos) && canPlace(board, newPos)) {
      moves.emplace_back(newPos);
    }
  }

  // capture left
  newPos = Position(row() + 1, col() - 1);
  if (!board.isEmpty(newPos) && canPlace(board, newPos)) {
    moves.emplace_back(newPos);
  }
  // capture right
  newPos = Position(row() + 1, col() + 1);
  if (!board.isEmpty(newPos) && canPlace(board, newPos)) {
    moves.emplace_back(newPos);
  }

  // en passant
  if (row() == 4 && board.fromType() == Type::B_PAWN
      && board.from().first == 6 && board.to().first == 4
      && abs<int>(board.from().second - col()) == 1) {
    moves.emplace_back(row() + 1, board.from().second);
  }
  return moves;
}

bool WPawn::canAttack(const Board& board, Position pos) const {
  int8_t dc = col() - pos.second;
  return (pos.first == row() + 1)  && (dc == 1 || dc == -1);
}

}  // namespace chess
