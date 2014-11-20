#ifndef B_PAWN_H
#define B_PAWN_H
#include "piece.h"

namespace chess {
class BPawn : public Piece {
  public:
    BPawn(int8_t row=-1, int8_t col=-1) : Piece(Type::B_PAWN, row, col) {}
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Position> getMoves(const Board& board) const;
};
}
#endif
