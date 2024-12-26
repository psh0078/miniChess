#include "bitboard.h"
#include <stdint.h>

Bitboard KING_ATTACKS[64];
Bitboard KNIGHT_ATTACKS[64];

typedef struct {
  uint64_t *ptr;
  uint64_t mask;
  uint64_t magic;
  int shift;
} SMagic;

SMagic bishop_masks[64];
SMagic rook_masks[64];
uint64_t bishop_attacks[64][4096];
uint64_t rook_attacks[64][4096];

// leapers
void init_king_attacks();
void init_knight_attacks();

// sliders (magic)
Bitboard rookMask(Square sq);
Bitboard bishopMask(Square sq);
Bitboard rookAttack(int square, Bitboard block);
Bitboard bishopAttack(Square square, Bitboard block);
int pop_1st_bit(Bitboard* bb);
uint64_t index_to_uint64(int index, int bits_in_mask, Bitboard attack_mask);

uint64_t xorshift64();
void init_random();
uint64_t random_64bit();

uint64_t findMagic(Square sq, int bishop);
void initMagics();
void init_sliders_attacks(int is_bishop);
Bitboard get_bishop_attacks(Square sq, Bitboard occupancy);
Bitboard get_rook_attacks(Square sq, Bitboard occupancy);
Bitboard get_queen_attacks(Square sq, Bitboard occupancy);
