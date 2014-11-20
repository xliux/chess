#include <cmath>

#include "bpawn.h"
#include "board.h"
#include "util.h"

namespace chess {
using namespace std;

vector<Position> BPawn::getMoves(const Board& board) const {
  vector<Position> moves;

  Position newPos(row() - 1, col());
  if (canPlace(board, newPos) && board.isEmpty(newPos)) {
    moves.emplace_back(newPos);
  }
  if (row() == 6 && board.isEmpty(newPos)) {
    newPos = Position(row() - 2, col());
    if (board.isEmpty(newPos) && canPlace(board, newPos)) {
      moves.emplace_back(newPos);
    }
  }

  // attack left
  newPos = Position(row() - 1, col() + 1);
  if (!board.isEmpty(newPos) && canPlace(board, newPos)) {
    moves.emplace_back(newPos);
  }
  // attack right
  newPos = Position(row() - 1, col() - 1);
  if (!board.isEmpty(newPos) && canPlace(board, newPos)) {
    moves.emplace_back(newPos);
  }

  // en passant
  if (row() == 3 && board.fromType() == Type::W_PAWN
      && board.from().first == 1 && board.to().first == 3
      && abs<int8_t>(board.from().second - col()) == 1) {
    moves.emplace_back(row() - 1, board.from().second);
  }
  return moves;
}

bool BPawn::canAttack(const Board& board, Position pos) const {
  int8_t dc = col() - pos.second;
  return (pos.first == row() - 1)  && (dc == 1 || dc == -1);
}

}  // namespace chess
