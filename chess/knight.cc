#include <stdlib.h>
#include <utility>
#include <vector>

#include "chess/knight.h"

#include "chess/board.h"
#include "chess/move.h"
#include "chess/util.h"

namespace chess {
using namespace std;
vector<Move> Knight::getMoves(const Board& board) const {
  static const DeltaMove kMoves[8] = {
    {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, {2, 1}
  };
  vector<Move> moves;
  moves.reserve(20);
  for (int i = 0; i < 8; ++i) {
    Position newPos = {row() + kMoves[i].dr, col() + kMoves[i].dc};
    if (canPlace(board, newPos)) {
      moves.emplace_back(board, Move::NORMAL_MOVE, position(), newPos);
    }
  }
  return moves;
}

bool Knight::canAttack(const Board& board, Position target) const {
  Position current = position();
  int8_t dr = target.first - current.first;
  int8_t dc = target.second - current.second;
  int area = dr * dc;
  return area == 2 || area == -2;
}

}  // namespace chess
