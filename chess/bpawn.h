#ifndef B_PAWN_H
#define B_PAWN_H

#include <vector>
#include "piece.h"

namespace chess {
class BPawn : public Piece {
  public:
    BPawn(int8_t row=-1, int8_t col=-1) : Piece(Type::B_PAWN, row, col) {}
    bool canAttack(const Board& board, Position pos) const;

  protected:
    std::vector<Move> getMoves(const Board& board) const;

  private:
    void addMove(const Board& board, Position newPos, std::vector<Move>* moves)
      const;
};
}
#endif
