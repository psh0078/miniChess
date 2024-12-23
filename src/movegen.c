#include <stdbool.h>
#include <time.h>

#include "movegen.h"
#include "bitboard.h"

int BISHOP_IDX_BITS[64] = {
    58, 59, 59, 59, 59, 59, 59, 58,
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59,
    58, 59, 59, 59, 59, 59, 59, 58
};

int ROOK_IDX_BITS[64] = {
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    52, 53, 53, 53, 53, 53, 53, 52
};

void init_king_attacks() {
  const int KingDelta[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1},  {1, 0},  {1, 1}
  };

  for (Square sq = 0; sq < 64; sq++) {
    Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    for (int i = 0; i < 8; i++) {
      int new_rank = rank + KingDelta[i][0];
      int new_file = file + KingDelta[i][1];

      if (new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8) {
        Square target = new_rank * 8 + new_file;
        set_bit(&attacks, target);
      }
    }
    KING_ATTACKS[sq] = attacks;
  }
}

void init_knight_attacks() {
  const int KnightDelta[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2},  {1, 2},  {2, -1},  {2, 1}
  };

  for (Square sq = 0; sq < 64; sq++) {
    Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;
    for (int i = 0; i < 8; i++) {
      int new_rank = rank + KnightDelta[i][0];
      int new_file = file + KnightDelta[i][1];
      if (new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8) {
        Square target = new_rank * 8 + new_file;
        set_bit(&attacks, target);
      }
    }
    KNIGHT_ATTACKS[sq] = attacks;
  }
}

uint64_t xorshift64_state = 1804289383;

uint64_t xorshift64() {
  uint64_t x = xorshift64_state;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  return xorshift64_state = x;
}

void init_random() {
  xorshift64_state = time(NULL);
}

Bitboard random_64bit() {
  return xorshift64() & xorshift64() & xorshift64() & xorshift64();
}

// count bits (Brian Kernighan's way)
int count_bits(Bitboard bitboard) {
  int count = 0;
  while (bitboard)
  {
      count++;
      bitboard &= bitboard - 1;
  }
  return count;
}

Bitboard rookMask(Square sq) {
  Bitboard attacks = 0ULL;
  int f, r;
  int tr = sq / 8;
  int tf = sq % 8;

  for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
  for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
  for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
  for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

  return attacks;
}

Bitboard bishopMask(Square sq) {
  Bitboard attacks = 0ULL;
  int f, r;
  int tr = sq / 8;
  int tf = sq % 8;

  for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
  for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

  return attacks;
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
