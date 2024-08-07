#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned long long U64;

void print_board(U64 bitboard) 
{
  for(int rank=0; rank < 8; rank++) {
    for(int file=0; file < 8; file++) {
      int square = rank * 8 + file;
      printf("%d", (bitboard & (1ULL << square)) ? 1 : 0);
    }
    printf("\n");
  } 
}

uint64_t square_to_bitboard(const char *square)
{
  int file = square[0] - 'a';
  int rank = square[1] - '1';
  int position = rank * 8 + file;
  return 1ULL << position;
}

bool is_empty(U64 bitboard, const char *square)
{
  uint64_t mask = square_to_bitboard(square);
  return (bitboard & mask) == 0;
}

void set_piece(const char *position, U64 *bitboard)
{
  if (!is_empty(*bitboard, position))
  {
    printf("The square is occupied.\n");
  }
  else 
  {
    printf("setting piece..\n");
    uint64_t mask = square_to_bitboard(position);
    *bitboard |= mask; 
  }
}

void clear_square(const char *position, U64 *bitboard)
{
  if (is_empty(*bitboard, position))
  {
    printf("This square is already empty.\n");
  }
  else
  {
    uint64_t mask = square_to_bitboard(position);
    *bitboard &= ~mask;
  }
}

int main() 
{
  printf("U64 board\n");
  U64 bitboard = 1ULL;
  set_piece("a2", &bitboard);
  clear_square("a2", &bitboard);
  print_board(bitboard);
  return 0;
}

