#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <stdint.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "folly/Checksum.h"
#include "folly/Hash.h"

#include "chess/move.h"
#include "chess/piece.h"

namespace chess {
enum {
  BOARD_SIZE = 8,
  NUM_DIAGS = 2 * BOARD_SIZE - 1
};
enum {
  NONE     = 0,
  CHECK_HEURISTIC = 10,
  STALLMATE = -2,
  CHECKMATE = 100
};

class Board {
  public:
    static constexpr Position invalidPosition() { return {-1, -1}; }
    static bool isValidPosition(Position pos) {
      return pos.first >= 0 && pos.first < BOARD_SIZE
        && pos.second >= 0 && pos.second < BOARD_SIZE;
    }

    // init a board with all pieces at the openning
    explicit Board(bool init_it = true);
    explicit Board(const std::string& loadFromFile);

    void add(Piece::Type type, Position pos);
    void add(Piece::Type type, int8_t row, int8_t col) { add(type, {row, col});}

    void set(Piece::Type type, Position pos) {
      setInternal(type, pos.first, pos.second);
      setKingPositionIfNeeded(type, pos);
    }

    void remove(int8_t row, int8_t col) {
      setInternal(Piece::Type::EMPTY, row, col);
    }

    Piece::Type getPieceTypeAtPosition(int8_t row, int8_t col) const {
      return positions_[row][col];
    }

    Piece::Type getPieceTypeAtPosition(Position pos) const {
      return getPieceTypeAtPosition(pos.first, pos.second);
    }

    bool isEmpty(Position pos) const {
      return getPieceTypeAtPosition(pos) == Piece::Type::EMPTY;
    }

    Position blackKingPosition() const { return blackKing_; }
    Position whiteKingPosition() const { return whiteKing_; }

    std::unique_ptr<Piece> blackKing() const {
      return piece(blackKingPosition());
    }
    std::unique_ptr<Piece> whiteKing() const {
      return piece(whiteKingPosition());
    }

    bool isEnPassantMove(Position fromPos, Position toPos) const;
    bool isCastlingMove(Position fromPos, Position toPos) const;

    // Is it now a check for the black/white.
    bool isChecked(bool black) const;

    std::unique_ptr<Board> clone() const;
    std::unique_ptr<Board> makeMove(
        Position pos, Position newPos, bool* pawnPromotion) const;
    std::unique_ptr<Board> makeMove(const Move& move) const;

    // for search on the BFS tree
    struct Node;
    typedef std::vector<Node> Layer;

    int evaluate(bool blackFirst, int maxDepth,
        std::vector<Layer>* solutionTree) const;

    int score() const { return score_; }
    int setScore(int score) { return (score_ = score); }

    // the last move piece type
    const Move& lastMove() const { return lastMove_; }
    Piece::Type fromType() const { return lastMove().fromType(); }
    Position from() const { return lastMove().from(); }
    // to position for the last move
    Position to() const { return lastMove().to(); }
    Piece::Type toType() const { return lastMove().toType(); }
    bool isEnpassantMove(Position from, Position to) const;

    // expand current node by evaluating the score for the current board state,
    // and taking all possible moves to generate all possible states in the next
    // layer.
    int expand(bool blackFirst, int layerNum, Layer* nextStates);

    std::unique_ptr<Piece> piece(Position pos) const {
      return Piece::create(getPieceTypeAtPosition(pos), pos);
    }

    const Piece* safePiece(Position pos) const;
    std::string printLastMove() const;

    uint64_t hash() const { 
      const uint8_t* start = 
        reinterpret_cast<const uint8_t*>(positions_[0]); 
      constexpr size_t halfSize = 
        sizeof(Piece::Type) * BOARD_SIZE * BOARD_SIZE / 2;
      const uint8_t* mid = start + halfSize;
      return std::hash<std::pair<uint32_t, uint32_t>>()(
          {folly::crc32c(start, halfSize), folly::crc32c(mid, halfSize)});
    }

    int compare(const Board& rhs) const {
      return memcmp(positions_[0], rhs.positions_[0], 
          sizeof(*positions_[0]) * BOARD_SIZE * BOARD_SIZE);
    }

  protected:
    void setKingPositionIfNeeded(Piece::Type type, Position pos) {
      switch (type) {
        case Piece::Type::B_KING: blackKing_ = pos; break;
        case Piece::Type::W_KING: whiteKing_ = pos; break;
        default: return;
      }
    }

    // current black king and white king positions
    Position blackKing_, whiteKing_;
    Piece::Type positions_[BOARD_SIZE][BOARD_SIZE];

  private:
    void setInternal(Piece::Type type, int8_t row, int8_t col) {
      positions_[row][col] = type;
    }

    void init();
    void clear();

    int8_t score_;
    Move lastMove_;
};

struct Board::Node {
  Board* parent;
  std::unique_ptr<Board> nextState;

  Node() : parent(nullptr) {}
  Node(Node&& rhs) :
    parent(rhs.parent), nextState(std::move(rhs.nextState)) {}
  Node(Board* cur, std::unique_ptr<Board> next) :
    parent(cur), nextState(std::move(next)) {}
  Node(const Node&) = delete;
  Node& operator =(const Node&) = delete;
  std::string printMove() const;
};

// print solution tree in DFS manner.
void printSolutionTree(const std::vector<Board::Layer>& solution);

}
#endif
