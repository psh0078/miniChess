#include "bitboard.h"

Bitboard KING_ATTACKS[64];
Bitboard KNIGHT_ATTACKS[64];

Bitboard rookMask(Square sq);
Bitboard bishopMask(Square sq);
void init_king_attacks();
void init_knight_attacks();
