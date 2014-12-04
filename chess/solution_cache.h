#ifndef CHESS_SOLUTION_CACHE
#define CHESS_SOLUTION_CACHE 
#include <atomic>
#include <string>
#include <google/dense_hash_map>
#include <glog/logging.h>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/piece.h"
#include "chess/piece_bank.h"
#include "chess/static_forest.h"

namespace chess {

typedef StaticForest<Move> SolutionForest;

class SolutionCache {
  public:
    SolutionCache();
    typedef google::dense_hash_map<uint64_t, SolutionForest::Index> SolutionMap;
    typedef google::dense_hash_map<uint64_t, std::pair<int, int>> ScoreMap;

    typedef SolutionForest::Node Node;
    typedef SolutionForest::Index Index;

    const SolutionForest& solutions() const { return solutions_; }
    const Node& solution(const Board& board, bool solutionForMaxStep) const;

    bool getScoreFromMaxCache(uint64_t sig, int height, int* score) const { 
      return getScoreFromCache(true, sig, height, score);
    }
    bool getScoreFromMinCache(uint64_t sig, int height, int* score) const {
      return getScoreFromCache(false, sig, height, score);
    }
    bool updateMaxCache(uint64_t sig, int height, int score) {
      return updateCache(true, sig, height, score);
    }
    bool updateMinCache(uint64_t sig, int height, int score) {
      return updateCache(false, sig, height, score);
    }
    Index getSolution(uint64_t sig) const {
      auto it = solutionMap_.find(sig);
      if (it == solutionMap_.end()) return SolutionForest::NULL_INDEX;
      ++solutionHits_;
      return it->second;
    }
    Index addSolutionNode(uint64_t key, const Board& board) {
      Index idx = solutions_.add(board.lastMove());
      return solutionMap_.insert(std::make_pair(key, idx)).first->second;
    }
    Index addSolutionNode(const Board& board) {
      return solutions_.add(board.lastMove());
    }

    void addSolutionChild(Index parent, Index child) {
      return solutions_.addChild(parent, child);
    }

    size_t memsize() const;
    std::string stats() const;

    void clear() {
      solutions_.clear();
      maxSignatures_.clear();
      minSignatures_.clear();
      gNumMaxStatesExpanded_ = 0;
      gNumMinStatesExpanded_ = 0;
      solutionMap_.clear();
      solutionHits_ = 0;
    }

    mutable std::atomic<int64_t> gNumMaxStatesExpanded_, gNumMinStatesExpanded_;

  private:
    bool getScoreFromCache(
        bool maxOrMin, uint64_t sig, int height, int* score) const;
    bool updateCache(bool maxOrMin, uint64_t sig, int height, int score);

    SolutionForest solutions_;
    SolutionMap solutionMap_;

    mutable std::atomic<int64_t> solutionHits_;
    ScoreMap maxSignatures_, minSignatures_;
};

}
#endif
