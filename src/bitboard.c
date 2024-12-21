#include <complex.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "bitboard.h"

void print_bitboard(Bitboard bitboard)
{
  printf("\n");
  for(int rank=7; rank >= 0; rank--) {
    printf("  %d ", rank + 1);
    for(int file=0; file < 8; file++) {
      int square = rank * 8 + file;
      printf(" %d", get_bit(bitboard, square) ? 1 : 0);
    }
    printf("\n");
  }
  printf("\n     a b c d e f g h\n\n");
  printf("Bitboard: %llud\n\n", bitboard);
}

int main()
{
  printf("Bitboard board\n");
  Board board;
  memset(&board, 0, sizeof(Board));

  set_bit(&board.pawns, a2);
  print_bitboard(board.pawns);

  return 0;
}
