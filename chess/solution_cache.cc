#include <climits>
#include <gflags/gflags.h>

#include "chess/solution_cache.h"

DEFINE_int32(min_store_level, 2, "min level to start store signature");

namespace chess {
using namespace std;

SolutionCache::SolutionCache() {
  maxSignatures_.set_empty_key(-1);
  minSignatures_.set_empty_key(-1);
  solutionMap_.set_empty_key(-1);
}

bool SolutionCache::getScoreFromCache(
    bool maxOrMin, uint64_t sig, int height, int* score) const {
  CHECK_NOTNULL(score);
  auto& signatures = maxOrMin? maxSignatures_ : minSignatures_;
  auto it = signatures.find(sig);
  if (it == signatures.end()) return false;
  if (it->second.first >= height) {
    *score = NONE;
    return true;
  } 
  if (it->second.second <= height) {
    *score = CHECKMATE;
    return true;
  }
  return false;
}

bool SolutionCache::updateCache(
    bool maxOrMin, uint64_t sig, int height, int score) {
  if (height < FLAGS_min_store_level) return false;

  auto& signatures = maxOrMin? maxSignatures_ : minSignatures_;
  auto it = signatures.find(sig);
  if (it != signatures.end()) { 
    if (score < CHECKMATE && it->second.first < height) {
      it->second.first = height;
    } else if (it->second.second > height) {
      it->second.second = height;
    }
  } else {
    if (score < CHECKMATE) {
      signatures.insert({sig, {height, numeric_limits<int>::max()}});
    } else {
      signatures.insert({sig, {-numeric_limits<int>::max(), height}});
    }
  }
  return false;
}

string SolutionCache::stats() const {
  return string("Total max states visited: ") 
    + std::to_string(gNumMaxStatesExpanded_) + "\n"
    + "Total min states visited: "
    + std::to_string(gNumMinStatesExpanded_) + "\n"
    + "Total max states stored: " 
    + std::to_string(maxSignatures_.size()) + "\n"
    + "Total min states stored: " 
    + std::to_string(minSignatures_.size()) + "\n"
    + "Solution forest nodes: " + to_string(solutions_.size()) + "\n"
    + "solution map size: " + std::to_string(solutionMap_.size())
    + " hits:" + to_string(solutionHits_);
}

}
