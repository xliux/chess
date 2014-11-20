#include <string>
#include <vector>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "chess/board.h"
#include "chess/piece.h"
#include "chess/king.h"
#include "chess/one_round.h"
#include "chess/solution_cache.h"

using namespace std;
using namespace chess;

DEFINE_string(board, "", "specifies the board file to load from.");
DEFINE_string(first, "black", "black|white: who moves first");
DEFINE_int32(moves, 2, "number of moves to look ahead.");

int main(int argc, char** argv) {
  FLAGS_logtostderr = true;
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  LOG(INFO) << "size of king: " << sizeof(King) << " board:" << sizeof(Board)
    << " node" << sizeof(Board::Node) << std::endl
    << " solutionmap: " << sizeof(SolutionCache::SolutionMap::value_type);

  bool blackFirst = FLAGS_first == "black";
  Board board(FLAGS_board);
  unique_ptr<OneRound> root(new OneRound(&board, blackFirst, 0));
  int score = root->expand(FLAGS_moves);
  LOG(INFO) << "score = " << score << std::endl << OneRound::cache().stats();
  LOG_IF(INFO, true || score == CHECKMATE)
    << "path:" << endl << root->printPath();
  return 0;
}
