#ifndef KNIGHT_H
#define KNIGHT_H
#include "piece.h"

namespace chess {
class Knight : public Piece {
  public:
    Knight(bool isBlack=true, int8_t row=-1, int8_t col=-1) :
      Piece(Type::EMPTY, row, col) {
        type_ = isBlack ? Type::B_KNIGHT: Type::W_KNIGHT;
      }
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Position> getMoves(const Board& board) const;
};

}  // namespce chess
#endif // KNIGHT_H
