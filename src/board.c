#include <stdio.h>

typedef unsigned long long U64;

void print_board(U64 bitboard) {
  for(int rank=0; rank < 8; rank++) {
    for(int file=0; file < 8; file++) {
      int idx = rank * 8 + file;
      printf(" %d ", idx);
    }
    printf("\n");
  } 
}


int main() {
  printf("U64 board\n");
  U64 bitboard = 0ULL;
  print_board(bitboard);
  return 0;
}
