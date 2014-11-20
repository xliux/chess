#ifndef ROOK_H
#define ROOK_H
#include "piece.h"

namespace chess {
class Rook : public Piece {
  public:
    Rook(bool isBlack=true, int8_t row=-1, int8_t col=-1) :
      Piece(Type::EMPTY, row, col) {
        type_ = isBlack ? Type::B_ROOK: Type::W_ROOK;
      }
    bool canAttack(const Board& board, Position pos) const;

protected:
    std::vector<Position> getMoves(const Board& board) const;
};

}  // namespce chess
#endif
