#include <stdbool.h>
#include "movegen.h"

void init_king_attacks() {
  const int KingDelta[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1},  {1, 0},  {1, 1}
  };

  for (Square sq = 0; sq < 64; sq++) {
    Bitboard bb = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    for (int i = 0; i < 8; i++) {
      int new_rank = rank + KingDelta[i][0];
      int new_file = file + KingDelta[i][1];

      if (new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8) {
        Square target = new_rank * 8 + new_file;
        set_bit(&bb, target);
      }
    }

    KING_ATTACKS[sq] = bb;
  }
}

void add_move(MoveList* moves, Square source, Bitboard board, Bitboard allies) {
  clear_bit(&board, allies);
  while (board) {
    Square target = __builtin_ctzll(board);

    moves->moves[moves->size].source = source;
    moves->moves[moves->size].target = target;
    moves->size++;

    board &= board - 1;
  }
}

// MoveList generate_moves(Board* board) {
//   MoveList result = {0};
//   insert_king_moves(board, &result);
//   return result;
// }
