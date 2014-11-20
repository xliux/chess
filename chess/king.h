#ifndef KING_H
#define KING_H
#include "piece.h"

namespace chess {
class King : public Piece {
  public:
    King(bool isBlack=true, int8_t row=-1, int8_t col=-1) :
      Piece(Type::EMPTY, row, col) {
        type_ = isBlack ? Type::B_KING : Type::W_KING;
      }
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Position> getMoves(const Board& board) const;
};
}

#endif
