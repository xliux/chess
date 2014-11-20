#include <stdlib.h>
#include <utility>
#include <vector>

#include "piece.h"
#include "board.h"
#include "king.h"
#include "util.h"

namespace chess {
using namespace std;

vector<Position> King::getMoves(const Board& board) const {
  vector<Position> moves;
  for (int dr = -1; dr <= 1; ++dr) {
    for (int dc = -1; dc <= 1; ++dc) {
      if (dr == 0 && dc == 0) continue;
      Position newPos(row() + dr, col() + dc);
      if (canPlace(board, newPos)) {
        moves.emplace_back(newPos);
      }
    }
  }
  return moves;
}

bool King::canAttack(const Board& board, Position pos) const {
  if (pos == position()) return false;
  int8_t dr = pos.first - row();
  int8_t dc = pos.second - col();
  return (dr <= 1 && dr >= -1) && (dc <= 1 && dc >= -1);
}

}  // namespace chess
