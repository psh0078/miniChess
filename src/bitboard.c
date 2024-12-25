#include <complex.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "bitboard.h"
#include "movegen.h"

bool get_bit(Bitboard bitboard, int square) {
  return (bitboard & BIT(square)) != 0;
}

void set_bit(Bitboard* bitboard, int square) {
  *bitboard |= BIT(square);
}

void clear_bit(Bitboard* bitboard, int square) {
  if (get_bit(*bitboard, square)) {
    *bitboard &= ~BIT(square);
  }
}

Bitboard shift(enum Direction D, Bitboard b) {
  return D == NORTH         ? b << 8
       : D == SOUTH         ? b >> 8
       : D == EAST          ? (b & ~FILE_H) << 1
       : D == WEST          ? (b & ~FILE_A) >> 1
       : D == NORTH_EAST    ? (b & ~FILE_H) << 9
       : D == NORTH_WEST    ? (b & ~FILE_A) << 7
       : D == SOUTH_EAST    ? (b & ~FILE_H) >> 7
       : D == SOUTH_WEST    ? (b & ~FILE_A) >> 9
       : 0;
}

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

  initMagics();
  init_sliders_attacks(1);
  init_sliders_attacks(0);
  return 0;
}
