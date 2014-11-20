#ifndef BISHOP_H
#define BISHOP_H
#include "piece.h"

namespace chess {
class Bishop : public Piece {
  public:
    Bishop(bool isBlack=true, int8_t row=-1, int8_t col=-1) :
      Piece(Type::EMPTY, row, col) {
        type_ = isBlack ? Type::B_BISHOP: Type::W_BISHOP;
      }
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Position> getMoves(const Board& board) const;
};

}  // namespce chess
#endif
