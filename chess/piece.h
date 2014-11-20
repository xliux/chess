#ifndef PIECE_H
#define PIECE_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

namespace chess {
class Board;

typedef std::pair<int8_t, int8_t> Position;
struct DeltaMove {
  int dr, dc;
};

inline static std::ostream& operator <<(std::ostream& os, Position pos) {
  os << "("<< (int)pos.first << "," << (int)pos.second << ")";
  return os;
}

static const char kPieceChar[] = "xprnkqbxxPRNKQB";

class Piece {
  public:
    enum class Type : std::int8_t {
      EMPTY  = 0,
      W_PAWN = 1,
      W_ROOK = 2,
      W_KNIGHT = 3,
      W_KING = 4,
      W_QUEEN = 5,
      W_BISHOP = 6,
      B_PAWN = W_PAWN + 8,
      B_ROOK = W_ROOK + 8,
      B_KNIGHT = W_KNIGHT + 8,
      B_KING = W_KING + 8,
      B_QUEEN = W_QUEEN + 8,
      B_BISHOP = W_BISHOP + 8,
    };

    Piece(Type type, int8_t row, int8_t col) :
      type_(type), row_(row), col_(col) {}
    virtual ~Piece() {}

    int8_t row() const { return row_; }
    int8_t col() const { return col_; }

    Position position() const { return Position{row(), col()}; }
    Type type() const { return static_cast<Type>(type_); }

    Piece* setPosition(Position pos) {
      row_ = pos.first;
      col_ = pos.second;
      return this;
    }

    static std::unique_ptr<Piece> create(Type type, Position pos);

    static bool isBlack(Piece::Type type) {
      return static_cast<int8_t>(type) > 8;
    }

    static bool isWhite(Piece::Type type) {
      int8_t value = static_cast<int8_t>(type);
      return value > 0 && value < 8;
    }

    static bool isSameColor(Type type1, Type type2) {
      if (type1 == Type::EMPTY || type2 == Type::EMPTY) return false;
      return isBlack(type1) == isBlack(type2);
    }

    // Returns all possible new states after the piece take one possible move.
    std::vector<std::unique_ptr<Board>> getNextStates(const Board& board) const;
    // Returns the next states generated.
    int getNextStates(
        const Board& board, std::vector<std::unique_ptr<Board>> *out) const;

    // This does a checkonly (without generating the next states) to see if
    // there is a valid move for the piece.
    // Returns false if there is no move the piece can make.
    bool hasNextStates(const Board& board) const;

    // Check if the piece can attack the position, assuming the position is
    // taken by an opponent piece.
    virtual bool canAttack(const Board& board, Position pos) const = 0;

    // estimate the heuristics of this piece to the king of the other side.
    float heuristicScore(const Board& board) const;

    bool equals(const Piece& other) const {
      return this->type() == other.type()
        && this->position() == other.position();
    }

    static std::string printPiece(Type type, Position pos);
    std::string print() const { return printPiece(type(), position()); }

    // Returns all moves the piece can potentially take. This does not check
    // if the king would be under attack after the move.
    virtual std::vector<Position> getMoves(const Board& board) const = 0;

  protected:
    friend class Board;
    friend class PieceBank;
    // friend class OneRound;

    // Check if the target position is reacheable by moving in a line
    // (defined in step) from src and nothing in between to block the move.
    static bool checkCanAttackLine(const Board& board,
        Position src, Position tgt, DeltaMove step);

    bool canPlace(const Board& board, Position newPos) const;

    void addMovesFromLine(const Board& board,
        DeltaMove step, std::vector<Position>* newPositions) const;

    float distanceFactor(Position other) const;

    Type type_;
    int8_t row_;
    int8_t col_;
};
}
#endif
