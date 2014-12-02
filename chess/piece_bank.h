#ifndef CHESS_PIECE_BANK_H
#define CHESS_PIECE_BANK_H

#include "chess/board.h"
#include "chess/piece.h"

#include "chess/bishop.h"
#include "chess/bpawn.h"
#include "chess/king.h"
#include "chess/knight.h"
#include "chess/queen.h"
#include "chess/rook.h"
#include "chess/wpawn.h"

namespace chess {

class PieceBank {
  typedef Piece::Type Type;
 public:
  static const Piece* get(Piece::Type type, Position pos) {
    return theBank_.piece(type, pos);
  }
  static const Piece* moveTo(const Piece* piece, Position pos) {
    return theBank_.piece(piece->type(), pos);
  }

 private:
  PieceBank() { init(); }

  const Piece* piece(Piece::Type type, Position pos) const {
    switch (type) {
      case Type::B_PAWN :  return &bpawns_[pos.first][pos.second];
      case Type::W_PAWN :  return &wpawns_[pos.first][pos.second];
      case Type::B_ROOK :  return &rooks_[0][pos.first][pos.second];
      case Type::W_ROOK :  return &rooks_[1][pos.first][pos.second];
      case Type::B_BISHOP: return &bishops_[0][pos.first][pos.second];
      case Type::W_BISHOP: return &bishops_[1][pos.first][pos.second];
      case Type::B_KNIGHT: return &knights_[0][pos.first][pos.second];
      case Type::W_KNIGHT: return &knights_[1][pos.first][pos.second];
      case Type::B_QUEEN:  return &queens_[0][pos.first][pos.second];
      case Type::W_QUEEN:  return &queens_[1][pos.first][pos.second];
      case Type::B_KING:   return &kings_[0][pos.first][pos.second];
      case Type::W_KING:   return &kings_[1][pos.first][pos.second];
      default: return nullptr;
    }
  }

  void init() {
    for (int row = 0; row < BOARD_SIZE; ++row) {
      for (int col = 0; col < BOARD_SIZE; ++col) {
        bpawns_[row][col].setPosition({row, col});
        wpawns_[row][col].setPosition({row, col});

        rooks_[0][row][col].setPosition({row, col});
        rooks_[0][row][col].type_ = Type::B_ROOK;
        rooks_[1][row][col].setPosition({row, col});
        rooks_[1][row][col].type_ = Type::W_ROOK;

        knights_[0][row][col].setPosition({row, col});
        knights_[0][row][col].type_ = Type::B_KNIGHT;
        knights_[1][row][col].setPosition({row, col});
        knights_[1][row][col].type_ = Type::W_KNIGHT;

        bishops_[0][row][col].setPosition({row, col});
        bishops_[0][row][col].type_ = Type::B_BISHOP;
        bishops_[1][row][col].setPosition({row, col});
        bishops_[1][row][col].type_ = Type::W_BISHOP;

        kings_[0][row][col].setPosition({row, col});
        kings_[0][row][col].type_ = Type::B_KING;
        kings_[1][row][col].setPosition({row, col});
        kings_[1][row][col].type_ = Type::W_KING;

        queens_[0][row][col].setPosition({row, col});
        queens_[0][row][col].type_ = Type::B_QUEEN;
        queens_[1][row][col].setPosition({row, col});
        queens_[1][row][col].type_ = Type::W_QUEEN;
      }
    }
  }

  static PieceBank theBank_;

  BPawn bpawns_[BOARD_SIZE][BOARD_SIZE];
  WPawn wpawns_[BOARD_SIZE][BOARD_SIZE];
  Rook rooks_[2][BOARD_SIZE][BOARD_SIZE];
  Bishop bishops_[2][BOARD_SIZE][BOARD_SIZE];
  King kings_[2][BOARD_SIZE][BOARD_SIZE];
  Queen queens_[2][BOARD_SIZE][BOARD_SIZE];
  Knight knights_[2][BOARD_SIZE][BOARD_SIZE];
};

}  // namespace chess

#endif
