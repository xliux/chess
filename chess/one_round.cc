#include <algorithm>
#include <vector>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "chess/one_round.h"
#include "chess/piece.h"
#include "chess/piece_bank.h"
#include "chess/solution_cache.h"

DECLARE_int32(min_store_level);

namespace chess {
using namespace std;

SolutionCache OneRound::cache_;

class TryMoveOnBoard {
  public:
    TryMoveOnBoard(Board* board, Position from, Position to) :
      board_(board), from_(from), to_(to), 
      fromType_(board->getPieceTypeAtPosition(from)),
      toType_(board->getPieceTypeAtPosition(to)) {
        enPassant_ = board_->isEnpassantMove(from, to);
        board_->set(Piece::Type::EMPTY, from_);
        board_->set(fromType_, to_);
        if (enPassant_) {
          Position enPassantPos = to_;
          if (fromType_ == Piece::Type::B_PAWN) {
            ++enPassantPos.first;
            CHECK(board->getPieceTypeAtPosition(enPassantPos) ==
                Piece::Type::W_PAWN) << enPassantPos;
          } else {
            CHECK(fromType_ == Piece::Type::W_PAWN);
            --enPassantPos.first;
            CHECK(board->getPieceTypeAtPosition(enPassantPos) ==
                Piece::Type::B_PAWN) << enPassantPos;
          }
          board_->set(Piece::Type::EMPTY, enPassantPos);
        }
      }

    ~TryMoveOnBoard() {
      board_->set(toType_, to_);
      board_->set(fromType_, from_);
      if (enPassant_) {
        if (fromType_ == Piece::Type::B_PAWN) {
          board_->set(Piece::Type::W_PAWN, {to_.first + 1, to_.second});
        } else {
          board_->set(Piece::Type::B_PAWN, {to_.first - 1, to_.second});
        }
      }
    }

  private:
    Board* board_;
    Position from_, to_;
    Piece::Type fromType_, toType_;
    bool enPassant_;
};

static bool isKing(Piece::Type type) {
  return type == Piece::Type::B_KING || type == Piece::Type::W_KING;
}

static Piece::Type opponentType(Piece::Type type) {
  //CHECK(type != Piece::Type::EMPTY);
  return Piece::isBlack(type) ? Piece::Type::W_KING : Piece::Type::B_KING;
}

static DeltaMove deltaMove(Position from, Position to) {
  return DeltaMove{
    static_cast<int8_t>(to.first - from.first),  
   static_cast<int8_t>(to.second - from.second)
  };
}

static bool canAttackLine(Piece::Type type, DeltaMove step) {
  int area = step.dr * step.dc;
  switch (type) {
    case Piece::Type::B_QUEEN: 
    case Piece::Type::W_QUEEN: return true;
    case Piece::Type::B_ROOK: 
    case Piece::Type::W_ROOK: return area == 0;
    case Piece::Type::B_BISHOP: 
    case Piece::Type::W_BISHOP: return area == 1 || area == -1;
    default: return false;
  }
}

bool OneRound::checkChecked(const Board& board, bool maxSide) const {
  Position king = (maxSide == blackFirst_) 
    ? board.blackKingPosition() : board.whiteKingPosition();
  const PieceList& pieces = maxSide ? minStepPieces_ : maxStepPieces_;
  for (auto& p : pieces) {
    // the last moved piece is not updated in the piece list.
    if (p->type() != board.getPieceTypeAtPosition(p->position())) continue;
    if (p->canAttack(board, king)) return true; 
  }
  return false;
}

bool OneRound::isChecking() const {
  Position king = blackFirst_ 
    ? curBoard_->whiteKingPosition() : curBoard_->blackKingPosition();
  // The max-move piece is updated need to check that.
  if (maxMovePiece_ != nullptr &&
      maxMovePiece_->canAttack(*maxStepState_, king)) {
      return true;
  }
  return checkChecked(*maxStepState_, false);
}

bool OneRound::isChecked() const {
  return checkChecked(*curBoard_, true);
}

bool OneRound::canMove(
    const Piece* piece, Position to, bool isChecked, Board* auxBoard) const {
  TryMoveOnBoard tryMove(auxBoard, piece->position(), to);
  auto type = piece->type();
  auto kingPos = Piece::isBlack(type)
    ? auxBoard->blackKingPosition() : auxBoard->whiteKingPosition();
  if (Board::isValidPosition(auxBoard->to()) && auxBoard->to() != to) {
    auto lastMovedPiece = auxBoard->safePiece(auxBoard->to());
    if (lastMovedPiece != nullptr 
        && lastMovedPiece->canAttack(*auxBoard, kingPos)) {
      return false;
    }
  }
  if (isChecked || isKing(type)) {
    return !checkChecked(*auxBoard, Piece::isBlack(type) == blackFirst_);
  }

  // Otherwise, check if the move can cause any indirect attacks
  DeltaMove dm = {piece->row() - kingPos.first, piece->col() - kingPos.second};
  if (dm.dr != dm.dc && dm.dr + dm.dc != 0 && dm.dr * dm.dc != 0) {
    return true;
  } 

  DeltaMove step = dm;
  if (step.dr != 0) step.dr = (step.dr > 0) ? 1 : -1;
  if (step.dc != 0) step.dc = (step.dc > 0) ? 1 : -1;
  // king of the current move side
  Position pos = Position(kingPos.first + step.dr, kingPos.second + step.dc); 
  for (; Board::isValidPosition(pos); 
      pos.first += step.dr, pos.second += step.dc) {
    if (pos == to) return true;  // block again
    if (auxBoard->isEmpty(pos) || pos == piece->position()) continue;
    Piece::Type firstPieceType = auxBoard->getPieceTypeAtPosition(pos);
    if (Piece::isSameColor(type, firstPieceType)) return true;
    return !canAttackLine(firstPieceType, step); 
  }
  return true;
}

void OneRound::init() {
  maxStepPieces_.clear();
  minStepPieces_.clear();
  Piece::Type moveFirst = blackFirst_ 
    ? Piece::Type::B_KING : Piece::Type::W_KING;
  for (int8_t row = 0; row < BOARD_SIZE; ++row) {
    for (int8_t col = 0; col < BOARD_SIZE; ++col) {
      if (curBoard_->isEmpty({row, col})) continue;
      auto piece = curBoard_->safePiece({row, col});
      if (Piece::isSameColor(moveFirst, piece->type())) {
        maxStepPieces_.emplace_back(piece);
      } else {
        minStepPieces_.emplace_back(piece);
      }
    }
  }
  VLOG(3) << "creating one round with " << maxStepPieces_.size() << " vs "
    << minStepPieces_.size() << " pieces";
  numMaxMoves_ = 0;

#ifdef DEBUG
  // make sure the current board is not done.
  if (VLOG_IS_ON(1)) {
    bool checked = checkChecked(*curBoard_, false);
    if (checked) {
      auto& board = *curBoard_;
      Position king = blackFirst_ ? 
        board.whiteKingPosition() : board.blackKingPosition();
      const PieceList& pieces = maxStepPieces_;
      LOG(ERROR) << "Checking checked for king " 
        << board.safePiece(king)->print();
      for (auto& p : pieces) {
        // the last moved piece is not updated in the piece list.
        if (p->type() != board.getPieceTypeAtPosition(p->position())) continue;
        if (p->canAttack(board, king)) {
          LOG(ERROR) << "king is checked by" << p->print() << std::endl
            << backwardPrintPath();
          break;
        }
      }
    }
    CHECK(!checked) 
      << "Board already in check: " << (blackFirst_ ? "white" : "black"); 
  }
#endif
}

string OneRound::backwardPrintPath() const {
  string output;
  auto* r = this;
  while (r) {
    output += r->curBoard_->printLastMove();
    r = r->parent_;
    if (r) {
      output += " <= " + r->maxStepState_->printLastMove() + "\n";
    }
  }
  return output;
}

const PieceList& OneRound::sameSidePieces(Piece::Type type) const {
  // CHECK(type != Piece::Type::EMPTY);
  return Piece::isBlack(type) == blackFirst_ ? maxStepPieces_ : minStepPieces_;
}

// Returns the pieces that are on the opponent side of the 'type'.
const PieceList& OneRound::opponentPieces(Piece::Type type) const {
  return sameSidePieces(opponentType(type));
}

float OneRound::calcMoveHeuristics(
    Board* auxBoard, const Piece* piece, Position to) const {
  VLOG(3) << "calc move heuristics from " << piece->print() << " to " << to;
  TryMoveOnBoard tryMove(auxBoard, piece->position(), to);

  float heuristics = 0; 
  auto type = piece->type();
  auto& pieces = sameSidePieces(type);
  for (auto& p : pieces) {
    if (p->position() == piece->position()) continue;
    // only need to consider the moves that changed the line attackers.
    if (canAttackLine(p->type(), 
          deltaMove(piece->position(), p->position()))) {
      heuristics = max(heuristics, p->heuristicScore(*auxBoard));
    }
  }

  if ((to.first == 0 && type == Piece::Type::B_PAWN) ||
      (to.first == BOARD_SIZE - 1 && type == Piece::Type::W_PAWN)) {
    auxBoard->set((Piece::isBlack(type)
          ? Piece::Type::B_QUEEN : Piece::Type::W_QUEEN), to);
    heuristics = std::max(heuristics, 
        auxBoard->safePiece(to)->heuristicScore(*auxBoard));
    // pawn promotion to knight
    auxBoard->set((Piece::isBlack(type)
          ? Piece::Type::B_KNIGHT : Piece::Type::W_KNIGHT), to);
    heuristics = std::max(heuristics,
        auxBoard->safePiece(to)->heuristicScore(*auxBoard));
  }

  return heuristics;
}

string OneRound::printMove(Position from, Position to) const {
  return Piece::printPiece(curBoard_->getPieceTypeAtPosition(from), to);
}

vector<pair<Position, Position>> OneRound::generateAndSortMaxMoves() {
  vector<float> estimates;
  vector<pair<Position, Position>> moves;
  auto auxBoard = curBoard_->clone(); // make a temporary variable
  for (auto& p : maxStepPieces_) {
    VLOG(2) << "check moves: " << p->print();
    Position from = p->position();
    for (auto& to : p->getMoves(*curBoard_)) {
      if (!this->canMove(p, to, isMaxChecked_, auxBoard.get())) continue;
      float est = PieceBank::moveTo(p, to)->heuristicScore(*auxBoard);
      est = max(est, calcMoveHeuristics(auxBoard.get(), p, to));
      moves.push_back(make_pair(from, to));
      estimates.push_back(est);
      VLOG(2) << "estimate move: " << from << " => " 
        << printMove(from, to) << " = " << est;
    }
  }
  vector<int> index(moves.size());
  for (int i = 0; i < moves.size(); index[i]=i, ++i) {}
  sort(index.begin(), index.end(),
      [&estimates](int x, int y) { return estimates[x] > estimates[y]; });
  vector<pair<Position, Position>> maxMoves(moves.size());
  for (int i = 0; i < index.size(); ++i) {
    maxMoves[i] = moves[index[i]];
    VLOG(2) << "adding move (" << i << "): " << maxMoves[i].first
      << " => " << printMove(maxMoves[i].first, maxMoves[i].second)
      << "  h=" << estimates[index[i]];
  }
  VLOG(2) << "total " << maxMoves.size() << " moves";
  return maxMoves;
}

struct Brackets {
  explicit Brackets(int loglevel) : loglevel_(loglevel) { 
    VLOG(loglevel) << "{"; 
  }
  ~Brackets() { VLOG(loglevel_) << "}"; }
  private:
  int loglevel_;
};

//Actually this should be the min solution.
SolutionForest::Index OneRound::storeSolution(uint64_t key) const {
  // store the current max state
  CHECK_NOTNULL(maxStepState_.get());
  auto parent = cache_.addSolutionNode(key, *maxStepState_);
  VLOG(1) << "store parent solution node: " << hex << key << dec
    << "  id=" << parent
    << " board=" << BoardStep::fromBoard(*maxStepState_).print();
  for (size_t i = 0; i < minStepStates_.size(); ++i) {
    auto idx = cache_.addSolutionNode(*minStepStates_[i]);
    VLOG(1) << "store solution min node: idx=" << idx << " i=" << i
      << " board=" << BoardStep::fromBoard(*minStepStates_[i]).print();
    cache_.addSolutionChild(parent, idx);
    if (i < minStepSolutionTrees_.size()) {
      auto h = minStepStates_[i]->hash();
      auto subTree = minStepSolutionTrees_[i];
      if (subTree != SolutionForest::NULL_INDEX) {
        cache_.addSolutionChild(idx, subTree);
        VLOG(1) << "store sub-solution tree: " << hex << h 
          << dec << "  par=" << idx << " subtree=" << subTree;
      }
    }
  }
  return parent;
}

int OneRound::expandMaxStep(int levelsLeft, 
    const vector<pair<Position, Position>>& maxMoves) {
  CHECK_GT(levelsLeft, 0);
  // if (levelsLeft <= 0) return NONE;
  VLOG(1) << "expanding max step: level=" << level_ << " total " 
    << maxMoves.size() << " moves";
  int moveIdx = 0;
  for (auto& move : maxMoves) {
    Brackets bracket(1);
    VLOG(1) << "---- [" << level_ << ":" << moveIdx++ << "/" << maxMoves.size()
      << "] max move@lvl " << level_ << "  "
      << curBoard_->safePiece(move.first)->print()
      << " => " << printMove(move.first, move.second);
    bool promotion = false;
    maxStepState_ = curBoard_->makeMove(move.first, move.second, &promotion);
    if (promotion) {  // first try replace it with Queeen
      maxStepState_->set((blackFirst_ 
            ? Piece::Type::B_QUEEN : Piece::Type::W_QUEEN), 
          move.second);
      maxMovePiece_ = maxStepState_->safePiece(move.second);

      ++cache_.gNumMaxStatesExpanded_;
      int score = NONE;
      uint64_t sig = maxStepState_->hash();
      cleanUpForMinStep();
      if (!cache_.getScoreFromMaxCache(sig, levelsLeft, &score)) {
        score = expandMinStep(levelsLeft);
        cache_.updateMaxCache(sig, levelsLeft, score);
      } else if (score == CHECKMATE) {
        // still need to expand the node to get the solutions
        score = expandMinStep(levelsLeft);
      }
      VLOG(2) << "++++promotion: try queen first... score=" << score;
      // min step expansion
      if (score == CHECKMATE) return CHECKMATE; 

      // try replace with a Knight
      VLOG(2) << "++++promotion: try knight second... ";
      maxStepState_->set((blackFirst_ 
            ? Piece::Type::B_KNIGHT : Piece::Type::W_KNIGHT),
          move.second);
      maxMovePiece_= maxStepState_->safePiece(move.second);
    } else {
      // Discard if the move forms an immediate loop without other state change.
      if (parent_ && parent_->maxStepState_->from() == move.second &&
          parent_->curBoard_->isEmpty(parent_->maxStepState_->to()) &&
          parent_->maxStepState_->to() == move.first) {
        VLOG(1) << "discard due to circular move";
        continue;
      }
      maxMovePiece_ = maxStepState_->safePiece(move.second);
    }
    ++cache_.gNumMaxStatesExpanded_;
    uint64_t sig = maxStepState_->hash();
    int score = NONE;
    cleanUpForMinStep();
    if (!cache_.getScoreFromMaxCache(sig, levelsLeft, &score)) {
      score = expandMinStep(levelsLeft);
      cache_.updateMaxCache(sig, levelsLeft, score);
    } else if (score == CHECKMATE) {
      // still need to expand the node to get the solutions
      score = expandMinStep(levelsLeft);
    }
    VLOG(1) << "---- move score=" << score; 
    if (score == CHECKMATE) return CHECKMATE; 
  }
  return NONE;
}

int OneRound::handleMinStepBoard(int levelsLeft, unique_ptr<Board> nextState) {
  int score = NONE;
  if (levelsLeft > 1) {
    SolutionForest::Index solution = SolutionForest::NULL_INDEX;
    unique_ptr<OneRound> child;
    uint64_t sig = nextState->hash();
    VLOG(1) << "check if solution exists for hash=" << hex << sig;
    if (!cache_.getScoreFromMinCache(sig, levelsLeft, &score)) {
      child.reset(new OneRound(nextState.get(), blackFirst_, level_ + 1, this));
      score = child->expand(levelsLeft - 1); 
      if (levelsLeft >= FLAGS_min_store_level) {
        VLOG(1) << "update min cache: lel=" << level_ << " hash=" << hex << sig;
        cache_.updateMinCache(sig, levelsLeft, score);
      }
      if (score == CHECKMATE) {
        CHECK_NOTNULL(child->maxStepState_.get());
        solution = child->storeSolution(sig);
        VLOG(1) << "storeed check solution: " << solution
          << "; hash=" << hex << sig;
      }
    } else if (score == CHECKMATE) {
      solution = cache_.getSolution(sig);
      VLOG(1) << "got score from cache: " << score << "; sol=" << solution
        << hex << " hash=" << sig;
    }
    ++cache_.gNumMinStatesExpanded_;
    if (score == CHECKMATE) {
      CHECK_NE(solution, SolutionForest::NULL_INDEX);
      minStepSolutionTrees_.emplace_back(solution);
    }
  }
  minStepStates_.emplace_back(std::move(nextState));
  return score;
}

int OneRound::makeOneMinMove(int levelsLeft, Position from, Position to) {
  bool promotion = false;
  auto next = maxStepState_->makeMove(from, to, &promotion);
  Brackets bracket(1);
  VLOG(1) << "=== [" << level_ << "] minstep move: " 
    << maxStepState_->safePiece(from)->print() << " => "
    << next->printLastMove();
  if (promotion) {
    VLOG(1) << "====== minstep promotion to Queen";
    next->set(blackFirst_ ? Piece::Type::W_QUEEN : Piece::Type::B_QUEEN, to);
    int score = handleMinStepBoard(levelsLeft, std::move(next));
    if (score < CHECKMATE) return score;

    // then try knight 
    next = minStepStates_.back()->clone();
    next->set(blackFirst_ ? Piece::Type::W_KNIGHT : Piece::Type::B_KNIGHT, to);
    VLOG(1) << "====== minstep promotion to Knight";
  }
  return handleMinStepBoard(levelsLeft, std::move(next));
}

int OneRound::expandMinStep(int levelsLeft) {
  isMinChecked_ = isChecking();
  VLOG(1) << "===  min step: level=" << level_ << (isMinChecked_? "*" :"");
  auto auxBoard = maxStepState_->clone();
  bool hasNextMove = false;
  for (auto& p : minStepPieces_) {
    // skip the piece that is already captured in the max move
    if (maxStepState_->getPieceTypeAtPosition(p->position()) != p->type()) {
      continue; 
    }
    Position from = p->position();
    for (auto& to : p->getMoves(*maxStepState_)) {
      if (!this->canMove(p, to, isMinChecked_, auxBoard.get())) continue;
      hasNextMove = true;
      int result = makeOneMinMove(levelsLeft, from, to);
      if (result < CHECKMATE) return result;
    }
  }
  VLOG_IF(1, isMinChecked_ || hasNextMove) 
    << ">>> checkmate at level: " << level_;
  return (isMinChecked_ || hasNextMove) ? CHECKMATE : STALLMATE;
}

int OneRound::expand(int levelsLeft) {
  CHECK_GT(levelsLeft, 0);
  isMaxChecked_ = isChecked();
  auto maxMoves = generateAndSortMaxMoves();
  numMaxMoves_ = maxMoves.size();
  if (numMaxMoves_ == 0) return isMaxChecked_ ? -CHECKMATE : -STALLMATE;
  int score = expandMaxStep(levelsLeft, maxMoves);
  cleanUp();
  return score;
}

static string printSolution(const SolutionForest& solutions,
    SolutionForest::Index root, const string& lead) {
  string output;
  auto& node = solutions.node(root);
  output.append(node.data_.print());
  output.append("  ");

  bool first = true;
  if (node.children_.empty()) output.append("#\n");
  for (SolutionForest::Index c : node.children_) {
    if (!first) output.append(lead + "      ");
    output.append(printSolution(solutions, c, lead + "      "));
    first = false;
  }
  return output;
}

string OneRound::printPath() const {
  string output;
  string lead;
  for (int i = 0; i < 2 * level_ + 1; ++i) lead.append("      ");
  output.append(maxStepState_->safePiece(maxStepState_->to())->print());
  output.append("   ");
  for (int i = 0; i < minStepStates_.size(); ++i) {
    if (i > 0) output.append(lead);
    output.append(
        minStepStates_[i]->safePiece(minStepStates_[i]->to())->print());
    output.append("   ");
    if (i < minStepSolutionTrees_.size()) {
      output.append(printSolution(
            cache_.solutions(), minStepSolutionTrees_[i], lead + "      "));
    }
  }
  return output;
}

}
