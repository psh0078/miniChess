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

Bitboard rookMask(Square sq);
Bitboard bishopMask(Square sq);
void init_king_attacks();
void init_knight_attacks();
Bitboard rookAttack(int square, Bitboard block);
Bitboard bishopAttack(Square square, Bitboard block);
uint64_t findMagic(Square sq, int bishop);
void initMagics();
void init_sliders_attacks(int is_bishop);
Bitboard get_bishop_attacks(Square sq, Bitboard occupancy);
Bitboard get_rook_attacks(Square sq, Bitboard occupancy);
