#include <stdlib.h>
#include <utility>
#include <vector>

#include "chess/rook.h"
#include "chess/board.h"
#include "chess/util.h"

namespace chess {
using namespace std;

vector<Position> Rook::getMoves(const Board& board) const {
  vector<Position> moves;
  addMovesFromLine(board, DeltaMove{0, 1}, &moves);
  addMovesFromLine(board, DeltaMove{0, -1}, &moves);
  addMovesFromLine(board, DeltaMove{1, 0}, &moves);
  addMovesFromLine(board, DeltaMove{-1, 0}, &moves);
  return moves;
}

bool Rook::canAttack(const Board& board, Position target) const {
  Position current = position();
  int8_t dr = target.first - current.first;
  int8_t dc = target.second - current.second;
  if (dr == 0) {  // same row
    return checkCanAttackLine(
        board, current, target, DeltaMove{0, dc > 0 ? 1 : -1});
  } else if (dc == 0) { // same column
    return checkCanAttackLine(
        board, current, target, DeltaMove{dr > 0 ? 1 : -1, 0});
  }
  return false;
}

}  // namespace chess
