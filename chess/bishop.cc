#include <stdlib.h>
#include <utility>

#include "chess/bishop.h"
#include "chess/board.h"
#include "chess/move.h"
#include "chess/util.h"

namespace chess {
using namespace std;

vector<Move> Bishop::getMoves(const Board& board) const {
  vector<Move> moves;
  moves.reserve(28);
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
