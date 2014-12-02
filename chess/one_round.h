#ifndef CHESS_ONE_ROUND_H
#define CHESS_ONE_ROUND_H

#include <atomic>
#include <memory>
#include <vector>
#include <google/dense_hash_map>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/piece.h"
#include "chess/solution_cache.h"

namespace chess {
typedef std::vector<const Piece*> PieceList;

// one possible Move (a step from both sides) with heuristics.
class OneRound {
  public: 
    explicit OneRound(const Board* curBoard, bool blackFirst, int level,
        const OneRound* parent=nullptr) 
      : curBoard_(curBoard), blackFirst_(blackFirst),
      level_(level), parent_(parent) { init(); }

    Piece::Type secondMoveKingType() const {
      return blackFirst_? Piece::Type::W_KING : Piece::Type::B_KING;
    }
    const PieceList& opponentPieces(Piece::Type type) const;
    const PieceList& sameSidePieces(Piece::Type type) const;

    // Expand the current board by DFS search with max-min steps.
    // Returns the min-max score of the current player.
    int expand(int maxLevel);

    // print the solution/current path
    std::string printPath() const;

    const OneRound* parent() const { return parent_; }

    static const SolutionCache& cache() { return cache_; }

  protected:
    void init();
    std::vector<Move> generateAndSortMaxMoves();
    int makeOneMinMove(int levelsLeft, const Move& move);
    int handleMinStepBoard(int levelsLeft, std::unique_ptr<Board> nextState);

    // Calculate the heuristic score of the move without actual 
    // generating the new board.
    float calcMoveHeuristics(
        Board* auxBoard, const Piece* piece, const Move& move) const;

    bool canMove(const Piece* piece, const Move& move,
        bool isChecked, Board* auxBoard) const;

    int expandMaxStep(int maxLevel, const std::vector<Move>& maxMoves);
    int expandMinStep(int maxLevel);

    SolutionForest::Index storeSolution(uint64_t key) const;

  private:
    bool isChecked() const;  // being checked by the opponent
    bool isChecking() const;  // is checking the opponent

    // General function to check if max/min side is a checked state
    bool checkChecked(const Board& board, bool maxSide) const; 
    void cleanUpForMinStep() {
      minStepStates_.clear();   
      minStepSolutionTrees_.clear();
      numMinMoves_ = 0;
    }
    void cleanUp() {
      maxStepPieces_.clear();
      minStepPieces_.clear();
    }

    bool isCircularMove(const Move& move) const;

    std::string printMove(Position from, Position to) const;
    std::string backwardPrintPath() const;

    static SolutionCache cache_;

    const Board* curBoard_;
    bool blackFirst_;
    int level_;
    std::vector<const Piece*> maxStepPieces_;
    std::vector<const Piece*> minStepPieces_;
    const Piece* maxMovePiece_;

    bool isMaxChecked_, isMinChecked_;
    int numMaxMoves_;
    std::unique_ptr<Board> maxStepState_;

    std::vector<std::unique_ptr<Board>> minStepStates_;
    std::vector<SolutionForest::Index> minStepSolutionTrees_;
    int numMinMoves_;
    const OneRound* parent_;
};
}

#endif
