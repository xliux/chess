#include <stdlib.h>
#include <utility>
#include <vector>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/queen.h"
#include "chess/util.h"

namespace chess {
using namespace std;

vector<Move> Queen::getMoves(const Board& board) const {
  vector<Move> moves;
  addMovesFromLine(board, DeltaMove{0, 1}, &moves);
  addMovesFromLine(board, DeltaMove{0, -1}, &moves);
  addMovesFromLine(board, DeltaMove{1, 0}, &moves);
  addMovesFromLine(board, DeltaMove{-1, 0}, &moves);
  addMovesFromLine(board, DeltaMove{-1, -1}, &moves);
  addMovesFromLine(board, DeltaMove{-1, 1}, &moves);
  addMovesFromLine(board, DeltaMove{1, -1}, &moves);
  addMovesFromLine(board, DeltaMove{1, 1}, &moves);
  return moves;
}

bool Queen::canAttack(const Board& board, Position target) const {
  Position current = position();
  int8_t dr = target.first - current.first;
  int8_t dc = target.second - current.second;
  if (dr == 0) {  // same row
    return checkCanAttackLine(
        board, current, target, DeltaMove{0, dc > 0 ? 1 : -1});
  } else if (dc == 0) { // same column
    return checkCanAttackLine(
        board, current, target, DeltaMove{dr > 0 ? 1 : -1, 0});
  } else if (dr == dc) {
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
