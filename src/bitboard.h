#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define BIT(sq) (1ULL << (sq))
#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (63 ^ __builtin_clzll(x))
#define POP_LSB(b, x) b = LSB(x); x &= ~BIT(b)
#define POP_MSB(b, x) b = MSB(x); x &= ~BIT(b)

#define RANK_1 0x00000000000000ffULL
#define RANK_2 0x000000000000ff00ULL
#define RANK_3 0x0000000000ff0000ULL
#define RANK_4 0x00000000ff000000ULL
#define RANK_5 0x000000ff00000000ULL
#define RANK_6 0x0000ff0000000000ULL
#define RANK_7 0x00ff000000000000ULL
#define RANK_8 0xff00000000000000ULL

#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_C 0x0404040404040404ULL
#define FILE_D 0x0808080808080808ULL
#define FILE_E 0x1010101010101010ULL
#define FILE_F 0x2020202020202020ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

enum Direction {
  NORTH, SOUTH, EAST, WEST,
  NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST
};

// LERF mapping (a1 ==> the least significant bit)
enum {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

enum { white, black, both};

typedef uint64_t Bitboard;
typedef uint8_t Square;

typedef struct {
  int stm;
  int xstm;
  Bitboard bitboards[12];
  Bitboard occupancies[3];
  Bitboard pieces[12];

  int castling;
  int enpassant;
} Board;

typedef struct {
  Square source;
  Square target;
  // flags
} Move;

typedef struct {
  Move moves[256];
  int size;
} MoveList;

enum {
  WHITE_PAWN,
  BLACK_PAWN,
  WHITE_KNIGHT,
  BLACK_KNIGHT,
  WHITE_BISHOP,
  BLACK_BISHOP,
  WHITE_ROOK,
  BLACK_ROOK,
  WHITE_QUEEN,
  BLACK_QUEEN,
  WHITE_KING,
  BLACK_KING
};

extern const int CHAR_TO_PIECE[]; 

bool get_bit(Bitboard bitboard, int square);
void set_bit(Bitboard* bitboard, int square);
void clear_bit(Bitboard* bitboard, int square);
int pop_1st_bit(Bitboard* bb);
Bitboard shift(enum Direction D, Bitboard b);

#endif
