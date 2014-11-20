#include <memory>

#include "chess/tests/util.h"
#include "chess/board.h"
#include "chess/piece.h"

namespace chess {
using namespace std;

unique_ptr<Board> createBoardForTest() {
  Board* board = new Board(false);
  board->add(Piece::Type::B_KING, 0, 0);
  board->add(Piece::Type::W_ROOK, 1, 1);
  board->add(Piece::Type::W_KING, 5, 5);
  return auto_ptr<Board>(board);
}

}
