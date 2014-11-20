#include <string>
#include <vector>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "chess/board.h"
#include "chess/king.h"
#include "chess/piece.h"

using namespace std;
using namespace chess;

DEFINE_string(board, "", "specifies the board file to load from.");
DEFINE_string(first, "black", "black|white: who moves first");
DEFINE_int32(moves, 2, "moves to look ahead");

int main(int argc, char** argv) {
  FLAGS_alsologtostderr = true;
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "size of king: " << sizeof(King) << " board:" << sizeof(Board) <<
    " node" << sizeof(Board::Node);

  Board board(FLAGS_board);
  vector<Board::Layer> solution;
  int score = board.evaluate(FLAGS_first == "black", FLAGS_moves * 2, &solution);
  if (score == CHECKMATE) {
    LOG(INFO) << (FLAGS_first == "black" ? "black" : "white") << " win! Steps:";
  } else if (score == -CHECKMATE) {
    LOG(INFO) << (FLAGS_first == "black" ? "black" : "white") << " loss! Steps:";
  } else {
    LOG(INFO) << "no solution within " << FLAGS_moves << "moves";
  }
  printSolutionTree(solution);

  // // Try dfs search
  // OneRound r(&board, FLAGS_first == "black", 0);
  // LOG(INFO) << "expand score=" << r.expand(2);
  return 0;
}
