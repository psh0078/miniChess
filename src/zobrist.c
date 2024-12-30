#include "random.h"
#include "zobrist.h"
#include "bitboard.h"

uint64_t ZOBRIST_PIECES[12][64];
uint64_t ZOBRIST_EP_KEYS[64];
uint64_t ZOBRIST_CASTLE_KEYS[16];
uint64_t ZOBRIST_SIDE_KEY;

void init_zobrist_keys() {
  for (int i = 0; i < 12; i++)
    for (int j = 0; j < 64; j++)
      ZOBRIST_PIECES[i][j] = random_64bit();

  for (int i = 0; i < 64; i++)
    ZOBRIST_EP_KEYS[i] = random_64bit();

  for (int i = 0; i < 16; i++)
    ZOBRIST_CASTLE_KEYS[i] = random_64bit();

  ZOBRIST_SIDE_KEY = random_64bit();
}

uint64_t generate_zobrist(Board* board) {
  uint64_t hash = 0ULL;
  for (int pc = WHITE_PAWN; pc <= BLACK_KING; pc++)  {
    Bitboard pieces = board->pieces[pc];
    while (pieces) {
      hash ^= ZOBRIST_PIECES[pc][pop_1st_bit(&pieces)];
    }
  }
  
  if (board->enpassant) {
    hash ^= ZOBRIST_EP_KEYS[board->enpassant];
  }
  
  hash ^= ZOBRIST_CASTLE_KEYS[board->castling];
  
  if (board->stm) hash ^= ZOBRIST_SIDE_KEY;

  return hash;
}
