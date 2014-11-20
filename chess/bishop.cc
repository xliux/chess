#include <stdlib.h>
#include <utility>

#include "bishop.h"
#include "board.h"
#include "util.h"

namespace chess {
using namespace std;

vector<Position> Bishop::getMoves(const Board& board) const {
  vector<Position> moves;
  addMovesFromLine(board, DeltaMove{-1, -1}, &moves);
  addMovesFromLine(board, DeltaMove{-1, 1}, &moves);
  addMovesFromLine(board, DeltaMove{1, -1}, &moves);
  addMovesFromLine(board, DeltaMove{1, 1}, &moves);
  return moves;
}

bool Bishop::canAttack(const Board& board, Position target) const {
  Position current = position();
  int8_t dr = target.first - current.first;
  int8_t dc = target.second - current.second;
  if (dr == dc) {
    int8_t step = dr > 0 ? 1 : -1;
    return checkCanAttackLine(board, current, target, DeltaMove{step, step});
  } else if (dr + dc == 0) {
    return (dr > 0)
      ? checkCanAttackLine(board, current, target, DeltaMove{1, -1})
      : checkCanAttackLine(board, current, target, DeltaMove{-1, 1});
  }
  return false;
}

}  // namespace chess
