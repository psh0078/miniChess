#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define clear_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard &= ~(1ULL << square)) : 0)

#define BIT(sq) (1ULL << (sq))
#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))

#define POP_LSB(b, x) b = LSB(x); x &= ~BIT(b);
#define POP_MSB(b, x) b = MSB(x); x &= ~BIT(b);

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
    NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST, NORTH2, SOUTH2
        
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

enum { white, black };

typedef unsigned long long U64;

typedef struct
{
  U64 whitePawns;
  U64 whiteRooks;
  U64 whiteKnights;
  U64 whiteBishops;
  U64 whiteQueens;
  U64 whiteKing;

  U64 blackPawns;
  U64 blackRooks;
  U64 blackKnights;
  U64 blackBishops;
  U64 blackQueens;
  U64 blackKing;

  U64 allWhitePieces;
  U64 allBlackPieces;
  U64 allPieces;
  
  U64 ep;
} Board;

U64 pawn_moves;
U64 knight_moves;
U64 king_moves;

U64 shift(enum Direction D, U64 b) {
  return D == NORTH         ? b << 8
       : D == SOUTH         ? b >> 8
       : D == NORTH2 ? b << 16
       : D == SOUTH2 ? b >> 16
       : D == EAST          ? (b & ~FILE_H) << 1
       : D == WEST          ? (b & ~FILE_A) >> 1
       : D == NORTH_EAST    ? (b & ~FILE_H) << 9
       : D == NORTH_WEST    ? (b & ~FILE_A) << 7
       : D == SOUTH_EAST    ? (b & ~FILE_H) >> 7
       : D == SOUTH_WEST    ? (b & ~FILE_A) >> 9
       : 0;
}
