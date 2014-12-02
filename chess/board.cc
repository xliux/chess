#include <cmath>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <glog/logging.h>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/piece.h"
#include "chess/piece_bank.h"

namespace chess {
using namespace std;
static const unordered_map<char, Piece::Type> kNotationToPieceType = {
  {'p', Piece::Type::W_PAWN},     {'P', Piece::Type::B_PAWN},
  {'r', Piece::Type::W_ROOK},     {'R', Piece::Type::B_ROOK},
  {'n', Piece::Type::W_KNIGHT},   {'N', Piece::Type::B_KNIGHT},
  {'k', Piece::Type::W_KING},     {'K', Piece::Type::B_KING},
  {'q', Piece::Type::W_QUEEN},    {'Q', Piece::Type::B_QUEEN},
  {'b', Piece::Type::W_BISHOP},   {'B', Piece::Type::B_BISHOP}
};

Board::Board(bool init_it) :
  blackKing_(-1, -1), whiteKing_(-1, -1), score_(NONE) {
    clear();
    if (init_it) {
      init();
    }
  }

Board::Board(const string& loadFromFile) :
  blackKing_(-1, -1), whiteKing_(-1, -1), score_(NONE) {
    clear();
    ifstream input(loadFromFile);

    LOG_IF(FATAL, !input) << "failed loading board: " << loadFromFile;
    LOG(INFO) << "loading board from: " << loadFromFile;
    string line;
    while (std::getline(input, line)) {
      if (line.empty() || line[0] == '#') continue;
      CHECK_EQ(3, line.size());
      auto iter = kNotationToPieceType.find(line[0]);
      CHECK(iter != kNotationToPieceType.end());
      int8_t col = line[1] - 'a';
      int8_t row = line[2] - '1';
      CHECK(row >= 0 && row < BOARD_SIZE);
      CHECK(col >= 0 && col < BOARD_SIZE);
      CHECK(isEmpty(Position(row, col))) << (int) row << (int)col;
      if (iter->second == Piece::Type::B_KING) {
        CHECK(blackKing_ == Position(-1, -1));
      } else if (iter->second == Piece::Type::W_KING) {
        CHECK(whiteKing_ == Position(-1, -1));
      }
      add(iter->second, row, col);
    }
    CHECK(blackKing_ != Position(-1, -1));
    CHECK(whiteKing_ != Position(-1, -1));
  }

void Board::clear() {
  memset(positions_[0],  0, sizeof(Piece::Type) * BOARD_SIZE * BOARD_SIZE);
}

void Board::init() {
  add(Piece::Type::B_ROOK, 7, 0);
  add(Piece::Type::B_KNIGHT, 7, 1);
  add(Piece::Type::B_BISHOP, 7, 2);
  add(Piece::Type::B_QUEEN, 7, 3);
  add(Piece::Type::B_KING, 7, 4);
  add(Piece::Type::B_BISHOP, 7, 5);
  add(Piece::Type::B_KNIGHT, 7, 6);
  add(Piece::Type::B_ROOK, 7, 7);
  for (auto c = 0; c < BOARD_SIZE; ++c) {
    add(Piece::Type::B_PAWN, 6, c);
  }

  add(Piece::Type::W_ROOK, 0, 0);
  add(Piece::Type::W_KNIGHT, 0, 1);
  add(Piece::Type::W_BISHOP, 0, 2);
  add(Piece::Type::W_QUEEN, 0, 3);
  add(Piece::Type::W_KING, 0, 4);
  add(Piece::Type::W_BISHOP, 0, 5);
  add(Piece::Type::W_KNIGHT, 0, 6);
  add(Piece::Type::W_ROOK, 0, 7);
  for (auto c = 0; c < BOARD_SIZE; ++c) {
    add(Piece::Type::B_PAWN, 1, c);
  }
}

void Board::add(Piece::Type type, Position pos) {
  CHECK(type != Piece::Type::EMPTY) << "type = " << static_cast<int>(type);
  DVLOG(3) << "adding: " << kPieceChar[static_cast<int>(type)]
    << " (" << char('a' + pos.second) << "" << char('1' + pos.second) << ")"
    << " type: " << (int)type << " row:" 
    << (int)pos.first << " col:" << (int)pos.second;
  set(type, pos);
}

const Piece* Board::safePiece(Position pos) const {
  return PieceBank::get(getPieceTypeAtPosition(pos), pos);
}

// For self-move, only need to check line attacks,
// e.g. R/B/Q from the opponent.
bool Board::isChecked(bool black) const {
  Piece::Type kingType = black ? Piece::Type::B_KING : Piece::Type::W_KING;
  Position kingPos = black ? blackKing_ : whiteKing_;
  for (int8_t row = 0; row < BOARD_SIZE; ++row) {
    for (int8_t col = 0; col < BOARD_SIZE; ++col) {
      Position pos(row, col);
      if (isEmpty(pos)) continue;
      Piece::Type otherType = getPieceTypeAtPosition(pos);
      if (Piece::isSameColor(kingType, otherType)) continue;
      auto opponent = Piece::create(otherType, pos);
      if (opponent->canAttack(*this, kingPos)) return true;
    }
  }
  return false;
}

unique_ptr<Board> Board::clone() const {
  return unique_ptr<Board>(new Board(*this));
}

// TODO: not strict. Can only apply after we know it is a valid move.
bool Board::isEnpassantMove(Position fromPos, Position toPos) const {
  auto fromType = getPieceTypeAtPosition(fromPos);
  if (fromType != Piece::Type::B_PAWN || fromType == Piece::Type::W_PAWN) {
    return false;
  }
  if (toPos.second == fromPos.second) return false;
  return isEmpty(toPos);
}

bool Board::isCastlingMove(Position fromPos, Position toPos) const {
  auto fromType = getPieceTypeAtPosition(fromPos);
  if (fromType != Piece::Type::W_KING || fromType != Piece::Type::B_KING) {
    return false;
  }
  return abs<int8_t>(toPos.second - fromPos.second) > 1;
}

unique_ptr<Board> Board::makeMove(const Move& move) const {
  auto cloned = this->clone();
  move.apply(cloned.get());
  cloned->lastMove_ = move;
  return std::move(cloned);
}

unique_ptr<Board> Board::makeMove(Position pos, Position newPos,
    bool* pawnPromotion) const {
  CHECK(isValidPosition(newPos));
  auto type = getPieceTypeAtPosition(pos);
  auto newBoard = this->clone();
  Move::SpecialType special = Move::NORMAL_MOVE;
  if ((type == Piece::Type::B_PAWN && newPos.first == 0)
    || (type == Piece::Type::W_PAWN && newPos.first == BOARD_SIZE - 1)) {
    special = Move::QUEEN_PROMOTION;
    *pawnPromotion = true;
  } else if (isEnpassantMove(pos, newPos)) {
    special = Move::PAWN_EN_PASSANT;
  } else if (isCastlingMove(pos, newPos)) {
    special = Move::KING_CASTLING;
  }
  Move move(*this, special, pos, newPos);
  move.apply(newBoard.get());
  newBoard->lastMove_ = move;
  return newBoard;
}

int Board::expand(bool blackMoveFirst, int layerNum, Layer *nextLayer) {
  CHECK_NOTNULL(nextLayer);
  if (layerNum % 2 == 0) {
    score_ = 1 + CHECKMATE;
  } else {
    score_ = NONE;
    if (isChecked(blackMoveFirst)) score_ = -CHECKMATE;
  }
  if (isChecked(!blackMoveFirst)) score_ = CHECKMATE;

  bool blackMoveCurrent = (layerNum % 2) ? blackMoveFirst : !blackMoveFirst;
  Piece::Type type = 
    blackMoveCurrent ? Piece::Type::B_KING : Piece::Type::W_KING;
  int numNextStates = 0;
  for (int8_t row = 0; row < BOARD_SIZE; ++row) {
    for (int8_t col = 0; col < BOARD_SIZE; ++col) {
      Position frm = {row, col};
      Piece::Type pieceType = getPieceTypeAtPosition(frm);
      if (!Piece::isSameColor(type, pieceType)) continue;
      auto p = Piece::create(pieceType, frm);
      for (auto& state : p->getNextStates(*this)) {
        VLOG(2) << "   ++++ adding new state: " << state.get();
        nextLayer->emplace_back(this, std::move(state));
        ++numNextStates;
      }
    }
  }
  return numNextStates;
}

int Board::evaluate(bool blackMoveFirst, int maxDepth,
    std::vector<Layer> *solutionTree) const {
  CHECK_NOTNULL(solutionTree);

  if (isChecked(!blackMoveFirst)) return CHECKMATE;
  vector<Layer> bfs(maxDepth + 1);

  // init root node
  bfs[0].emplace_back(nullptr, this->clone());
  // forward BFS expansion
  for (int i = 1; i < maxDepth + 1; ++i) {
    VLOG(1) << "======== Expanding level " << i << " ===========";
    for (auto& node : bfs[i - 1]) {
      VLOG(2) << "    expanding " << node.nextState.get()
        << "  last move=" << node.printMove() << " [lvl=" << i << "]";
      int numNextStates = node.nextState->expand(blackMoveFirst, i, &bfs[i]);
      VLOG(2) << "    expanded " << node.nextState.get()
        << "  score =" << node.nextState->score() << " #states=" << numNextStates;
    }
    LOG(INFO) << "layer " << i << " total states: " << bfs[i].size();
  }
  // backward min-max score propagation
  for (int i = maxDepth; i > 0; --i) {
    if (i % 2 == 1) { // useMaxOperation
      for (auto& node : bfs[i]) {
        node.parent->score_ = std::max(
            node.parent->score_, node.nextState->score_);
      }
    } else {
      for (auto& node : bfs[i]) {
        // No children nodes and the opponent is not in check.
        if (node.nextState->score_ > CHECKMATE) {
          node.nextState->score_ = STALLMATE;
        }
        node.parent->score_ = std::min(
            node.parent->score_, node.nextState->score_);
      }
    }
  }

  // re-traverse the tree to build the min-max paths
  unordered_set<const Board*> nodesOnPath;
  solutionTree->clear();
  solutionTree->resize(maxDepth + 1);
  nodesOnPath.insert(bfs[0][0].nextState.get());
  (*solutionTree)[0].emplace_back(std::move(bfs[0][0]));
  int score = (*solutionTree)[0][0].nextState->score();

  for (int i = 1; i <= maxDepth; ++i) {
    bool useMaxOperation = (i % 2 == 1);
    for (Node& node : bfs[i])
      if (nodesOnPath.count(node.parent) > 0
          && ((useMaxOperation && node.nextState->score() >= score)
            || (!useMaxOperation && node.nextState->score() <= score))) {
        nodesOnPath.insert(node.nextState.get());
        (*solutionTree)[i].emplace_back(std::move(node));
    }
  }
  return score;
}

string Board::printLastMove() const {
  if (!Board::isValidPosition(lastMove().from())) return "---";
  return lastMove().notation();
}

string Board::Node::printMove() const {
  return nextState->printLastMove();
}

void dfsPrintTree(const vector<Board::Layer>& tree,
    const unordered_map<const Board*, int>& startPos,
    int layerNo, int layerOff, int indent) {
  static const char kIndent[] = "%-16s";
  for (int i = 0; i < indent; ++i) printf(kIndent, "");

  const auto& node = tree[layerNo][layerOff];
  auto startPosIter = startPos.find(node.nextState.get());
  bool hasChild = (startPosIter != startPos.end()) 
    && (tree[layerNo + 1][startPosIter->second].parent == node.nextState.get());
  printf("[%01d]%3s(%5d)%c  ", layerNo, node.printMove().c_str(),
      node.nextState->score(), (hasChild ? '+' : '|'));
  if (hasChild) {
    int start = startPosIter->second;
    for (int j = start; j < tree[layerNo + 1].size(); ++j) {
      if (tree[layerNo + 1][j].parent != node.nextState.get()) break;
      dfsPrintTree(tree, startPos, layerNo + 1, j,
          j == start ? 0 : layerNo + 1);
    }
  }
  printf("\n");
}

void printSolutionTree(const vector<Board::Layer>& solution) {
  unordered_map<const Board*, int> startPos;
  for (auto& layer : solution) {
    for (int j = 0; j < layer.size(); ++j) {
      auto& node = layer[j];
      if (startPos.count(node.parent) == 0) startPos[node.parent] = j;
    }
  }
  dfsPrintTree(solution, startPos, 0, 0, 0);
}

}  // namespace chess
