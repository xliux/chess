#ifndef W_PAWN_H
#define W_PAWN_H
#include "piece.h"

namespace chess {
class WPawn : public Piece {
  public:
    WPawn(int8_t row=-1, int8_t col=-1) : Piece(Type::W_PAWN, row, col) {}
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Position> getMoves(const Board& board) const;
};
}
#endif
