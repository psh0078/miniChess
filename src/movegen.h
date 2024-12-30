#include "bitboard.h"
#include <stdint.h>

Bitboard PAWN_ATTACKS[2][64];
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
void init_pawn_moves();
Bitboard pawnAttacks(Square sq, int side);
void init_leapers_attacks();

// sliders (magic)
Bitboard rookMask(Square sq);
Bitboard bishopMask(Square sq);
Bitboard rookAttack(int square, Bitboard block);
Bitboard bishopAttack(Square square, Bitboard block);
uint64_t set_occupancy(int index, int bits_in_mask, Bitboard attack_mask);

uint64_t findMagic(Square sq, int bishop);
void initMagics();
void init_sliders_attacks(int is_bishop);
Bitboard get_bishop_attacks(Square sq, Bitboard occupancy);
Bitboard get_rook_attacks(Square sq, Bitboard occupancy);
Bitboard get_queen_attacks(Square sq, Bitboard occupancy);

// add all the possible moves from a square to a MoveList
void add_move(MoveList* moves, Square source, Square target);
void insert_king_moves(MoveList* moves, Bitboard allies);
void make_move(Move move, Board* board);
void generate_moves(MoveList* moves);

// MoveList generate_moves(Board* board) {
//   MoveList result = {0};
//   insert_king_moves(board, &result);
//   return result;
// }
