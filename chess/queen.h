#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"

namespace chess {
class Queen : public Piece {
  public:
    Queen(bool isBlack=true, int8_t row=-1, int8_t col=-1) :
      Piece(Type::EMPTY, row, col) {
        type_ = isBlack ? Type::B_QUEEN: Type::W_QUEEN;
      }
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Move> getMoves(const Board& board) const;
};

}  // namespce chess
#endif
