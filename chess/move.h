#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <string>
#include <utility>

#include "chess/piece.h"

namespace chess {
class Board;

class Move {
  public:
    enum SpecialType {
      NORMAL_MOVE = 0,
      QUEEN_PROMOTION = 1,
      KNIGHT_PROMOTION = 2,
      PAWN_EN_PASSANT = 3,
      KING_CASTLING = 4,
    };

    Move() : special_(NORMAL_MOVE), from_({-1, -1}), to_({-1, -1}),
        fromType_(Piece::Type::EMPTY), toType_(Piece::Type::EMPTY) {}

    Move(SpecialType special, Position from, Position to,
        Piece::Type fromType, Piece::Type toType) :
      special_(special), from_(from), to_(to),
      fromType_(fromType), toType_(toType) {};

    Move(const Board& board, SpecialType special, Position from, Position to);

    // only changes the board piece positions, 
    // not updating other fields (lastMove etc) in the board.
    void apply(Board* board) const;

    // the reverse of apply
    void undo(Board* board) const;

    Position from() const { return from_; }
    Position to() const { return to_; }
    Piece::Type fromType() const { return fromType_; }
    Piece::Type toType() const { return toType_; }
    SpecialType special() const { return special_; }

    Piece::Type newType() const {
      switch (special_) {
        case QUEEN_PROMOTION: return Piece::getSameQueenType(fromType_);
        case KNIGHT_PROMOTION: return Piece::getSameKnightType(fromType_);
        default: return fromType_;
      }
    }

    std::string print() const;
    std::string notation() const;
    float heuristicScore(const Board& board) const;

    // Get the rook positions (pre- and post-move) if it's a castling move;
    // This asserts the move is a King-castling move. 
    std::pair<Position,Position> getCastlingRookPos() const;

  private:
    void setupEnPassant(Board* board) const;
    void setupCastling(Board* board) const;
    void undoEnPassant(Board* board) const;
    void undoCastling(Board* board) const;

    SpecialType special_;
    Position from_, to_;
    Piece::Type fromType_, toType_;
};
}
#endif
