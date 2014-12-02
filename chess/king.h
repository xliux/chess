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
    Position origPos() const {
      return type_ == Type::B_KING ? Position(7, 4) : Position(0, 4);
    }
    bool canCastle(const Board& board, bool smaller) const;
    std::vector<Move> getMoves(const Board& board) const;
};
}

#endif
