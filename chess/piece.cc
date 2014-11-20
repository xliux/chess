#include <cassert>
#include <cmath>
#include <memory>
#include <glog/logging.h>

#include "piece.h"
#include "bishop.h"
#include "board.h"
#include "bpawn.h"
#include "king.h"
#include "knight.h"
#include "queen.h"
#include "rook.h"
#include "wpawn.h"

namespace chess {
using namespace std;

string Piece::printPiece(Type type, Position pos) {
  string move = "---";
  move[0] = kPieceChar[static_cast<int8_t>(type)];
  move[1] = 'a' + pos.second;
  move[2] = '1' + pos.first;
  return move;
}

bool Piece::canPlace(const Board& board, Position pos) const {
  if (!Board::isValidPosition(pos)) return false;
  return !isSameColor(type(), board.getPieceTypeAtPosition(pos));
}

unique_ptr<Piece> Piece::create(Type type, Position pos) {
  switch (type) {
    case Type::B_PAWN:
      return unique_ptr<Piece>(new BPawn(pos.first, pos.second));
    case Type::W_PAWN:
      return unique_ptr<Piece>(new WPawn(pos.first, pos.second));
    case Type::B_KING:
      return unique_ptr<Piece>(new King(true, pos.first, pos.second));
    case Type::W_KING:
      return unique_ptr<Piece>(new King(false, pos.first, pos.second));
    case Type::B_QUEEN:
      return unique_ptr<Piece>(new Queen(true, pos.first, pos.second));
    case Type::W_QUEEN:
      return unique_ptr<Piece>(new Queen(false, pos.first, pos.second));
    case Type::B_ROOK:
      return unique_ptr<Piece>(new Rook(true, pos.first, pos.second));
    case Type::W_ROOK:
      return unique_ptr<Piece>(new Rook(false, pos.first, pos.second));
    case Type::B_BISHOP:
      return unique_ptr<Piece>(new Bishop(true, pos.first, pos.second));
    case Type::W_BISHOP:
      return unique_ptr<Piece>(new Bishop(false, pos.first, pos.second));
    case Type::B_KNIGHT:
      return unique_ptr<Piece>(new Knight(true, pos.first, pos.second));
    case Type::W_KNIGHT:
      return unique_ptr<Piece>(new Knight(false, pos.first, pos.second));
    default: return unique_ptr<Piece>();
  }
}

vector<unique_ptr<Board>> Piece::getNextStates(const Board& board) const {
  vector<unique_ptr<Board>> nextStates;
  getNextStates(board, &nextStates);
  return nextStates;
}

bool Piece::hasNextStates(const Board& board) const {
  auto moves = getMoves(board);
  for (auto newPos : moves) {
    CHECK(Board::isValidPosition(newPos));
    bool promotion = false;
    auto next = board.makeMove(position(), newPos, &promotion);
    if (next->isChecked(isBlack(type()))) continue;
    return true;
  }
  return false;
}

int Piece::getNextStates(const Board& board, 
    vector<unique_ptr<Board>> *nextStates) const {
  CHECK_NOTNULL(nextStates);
  int origSize = nextStates->size();
  auto moves = getMoves(board);
  for (auto newPos : moves) {
    CHECK(Board::isValidPosition(newPos));
    bool promotion = false;
    unique_ptr<Board> next = board.makeMove(position(), newPos, &promotion);
    if (!next->isChecked(isBlack(type()))) {
      if (promotion) { // Pawn promotion to Queen
        unique_ptr<Board> queenPromo = next->clone();
        queenPromo->set(isBlack(type()) ? Type::B_QUEEN : Type::W_QUEEN,
            newPos);
        nextStates->push_back(std::move(queenPromo));
        VLOG(3) << " ---- add move: " << kPieceChar[(int)type()]
          << char('a' + newPos.second) << char('1' + newPos.first);

        // Knight promotion
        next->set(isBlack(type()) ? Type::B_KNIGHT: Type::W_KNIGHT, newPos);
      }
      VLOG(3) << " ---- add move: " << kPieceChar[(int)type()]
        << char('a' + newPos.second) << char('1' + newPos.first);
      nextStates->push_back(std::move(next));
    }
  }
  return nextStates->size() - origSize;
}

/* static */
bool Piece::checkCanAttackLine(
    const Board& board, Position src, Position tgt, DeltaMove step) {
  Position pos = src;
  do {
    pos.first += step.dr;
    pos.second += step.dc;
    if (!Board::isValidPosition(pos)) return false;
    if (tgt == pos) return true;
    if (!board.isEmpty(pos)) return false;
  }  while (true);
  return false;
}

void Piece::addMovesFromLine(
    const Board& board, DeltaMove step, vector<Position> *movePositions) const {
  assert(movePositions != nullptr);
  Position pos = {row() + step.dr, col() + step.dc};
  while (Board::isValidPosition(pos)) {
    if (!isSameColor(type(), board.getPieceTypeAtPosition(pos))) {
      movePositions->emplace_back(pos);
    }
    if (!board.isEmpty(pos)) break;
    pos.first += step.dr;
    pos.second += step.dc;
  }
}

float Piece::heuristicScore(const Board& board) const {
  Position opponentKingPos = isBlack(type()) 
    ? board.whiteKingPosition() : board.blackKingPosition(); 
  if (canAttack(board, opponentKingPos)) return CHECK_HEURISTIC;
  VLOG(3) << "dist factor: " << distanceFactor(opponentKingPos);

  return CHECK_HEURISTIC * distanceFactor(opponentKingPos);
}

float Piece::distanceFactor(Position other) const {
  int8_t dx = abs<int8_t>(other.first - row());
  int8_t dy = abs<int8_t>(other.second - col());
  int8_t distance = dx + dy;
  float factor = 1.0f;
  switch (this->type()) {
    case Type::B_QUEEN:
    case Type::W_QUEEN:
    {
      factor *= exp(-0.05f * distance);
      return (dx == 0 || dy == 0 || dx == dy) ? factor * 0.5f : factor * 0.25f;
    }
    case Type::B_ROOK:
    case Type::W_ROOK:
    {
      factor *= exp(-0.1f * distance);
      return (dx == 0 || dy == 0) ? factor * 0.5f : factor * 0.1f;
    }
    case Type::B_BISHOP:
    case Type::W_BISHOP: 
    {
      factor *= exp(-0.1f * distance);
      return dx == dy ? factor * 0.4f : factor * 0.1f;
    }
    case Type::B_KNIGHT:
    case Type::W_KNIGHT: return exp(-0.3f * distance);

    case Type::B_KING:
    case Type::W_KING: return exp(-1.0 * distance);

    case Type::B_PAWN:
    case Type::W_PAWN: return exp(-0.5 * distance);

    default: return 0;
  }
}

}
