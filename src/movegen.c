// Reference: https://www.chessprogramming.org/Magic_Bitboards

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

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

uint64_t xorshift64_state = 2304978537;

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

uint64_t random_64bit() {
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
  int f, r; // file, rank
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
  int f, r; // file, rank
  int tr = sq / 8;
  int tf = sq % 8;

  for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
  for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
  for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

  return attacks;
}

Bitboard bishopAttack(Square square, Bitboard block) {
  Bitboard result = 0ULL;
  int f, r; // file, rank
  int tr = square / 8;
  int tf = square % 8;

  for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
    result |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  }
  for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
    result |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  }
  for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
    result |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  }
  for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
    result |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  }
  return result;
}

Bitboard rookAttack(int square, Bitboard block) {
  Bitboard result = 0ULL;
  int f, r;
  int tr = square / 8;
  int tf = square % 8;

  for (r = tr + 1; r <= 7; r++) {
    result |= (1ULL << (r * 8 + tf));
    if (block & (1ULL << (r * 8 + tf))) break;
  }
  for (r = tr - 1; r >= 0; r--) {
    result |= (1ULL << (r * 8 + tf));
    if (block & (1ULL << (r * 8 + tf))) break;
  }
  for (f = tf + 1; f <= 7; f++) {
    result |= (1ULL << (tr * 8 + f));
    if (block & (1ULL << (tr * 8 + f))) break;
  }
  for (f = tf - 1; f >= 0; f--) {
    result |= (1ULL << (tr * 8 + f));
    if (block & (1ULL << (tr * 8 + f))) break;
  }
  return result;
}

int pop_1st_bit(uint64_t *b) {
  if (*b == 0) {
    return -1;
  }
  int index = __builtin_ctzll(*b);
  *b &= (*b - 1);
  return index;
}

uint64_t index_to_uint64(int index, int bits, uint64_t m) {
  uint64_t result = 0ULL;
  for(int i = 0; i < bits; i++) {
    int j = pop_1st_bit(&m);
    if(index & (1 << i)) result |= (1ULL << j);
  }
  return result;
}

uint64_t findMagic(Square sq, int bishop) {
  uint64_t b[4096], a[4096];
  uint64_t* used = bishop ? bishop_attacks[sq] : rook_attacks[sq];
  int i, fail;
  const int m = bishop ? BISHOP_IDX_BITS[sq] : ROOK_IDX_BITS[sq];
  uint64_t mask = bishop ? bishopMask(sq) : rookMask(sq);
  int n = count_bits(mask);

  for(i = 0; i < (1 << n); i++) {
    b[i] = index_to_uint64(i, n, mask);
    a[i] = bishop ?  bishopAttack(sq, b[i]) : rookAttack(sq, b[i]);
  }
  for(int k = 0; k < 100000000; k++) {
    uint64_t magic = random_64bit();
    if(count_bits((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
    for(i = 0; i < 4096; i++) used[i] = 0ULL;
    for (i = 0, fail = 0; !fail && i < (1 << n); i++) {
      int magic_index = (int)((b[i] * magic) >> m);
      if(used[magic_index] == 0ULL)
        used[magic_index] = a[i];
      else if(used[magic_index] != a[i]) fail = 1;
    }
    if (!fail) {
      if (bishop) {
        bishop_masks[sq].ptr = bishop_attacks[sq];
        bishop_masks[sq].mask = mask;
        bishop_masks[sq].magic = magic;
        bishop_masks[sq].shift = m;
      } else {
        rook_masks[sq].ptr = rook_attacks[sq];
        rook_masks[sq].mask = mask;
        rook_masks[sq].magic = magic;
        rook_masks[sq].shift = m;
      }
    }
  }
  return 0ULL;
}

void initMagics() {
  for (int i = 0; i < 64; i++) {
    findMagic(i, 1);
  }
  for (int i = 0; i < 64; i++) {
    findMagic(i, 0);
  }
}

Bitboard get_bishop_attacks(Square sq, Bitboard occupancy) {
  uint64_t* aptr = bishop_masks[sq].ptr;
	occupancy &= bishop_masks[sq].mask;
	occupancy *=  bishop_masks[sq].magic;
	occupancy >>= bishop_masks[sq].shift;
	return aptr[occupancy];
}

Bitboard get_rook_attacks(Square sq, Bitboard occupancy) {
  uint64_t* aptr = rook_masks[sq].ptr;
	occupancy &= rook_masks[sq].mask;
	occupancy *=  rook_masks[sq].magic;
	occupancy >>= rook_masks[sq].shift;
	return aptr[occupancy];
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
