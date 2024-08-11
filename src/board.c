#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"

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
  U64 castling;
  U64 sideToMove;
} Board;

void init_board(Board* board)
{
  board->allWhitePieces = board->whitePawns | board->whiteRooks | board->whiteKnights | board->whiteBishops | board->whiteQueens | board->whiteKing;
  board->allBlackPieces = board->blackPawns | board->blackRooks | board->blackKnights | board->blackBishops | board->blackQueens | board->blackKing;
  
}

U64 generate_pawn_moves(Board* board, int side)
{
  bool is_white = (side == 0);
  U64 pawn_moves = 0ULL;
  U64 pawns = is_white ? board->whitePawns : board->blackPawns; 
  U64 all_pieces = board->allWhitePieces | board->allBlackPieces;
  U64 pawn_square;
  // push moves
  while (pawns) {
    POP_LSB(pawn_square, pawns);

    // one-push
    U64 push_target = is_white ? BIT(pawn_square) << 8 : BIT(pawn_square) >> 8; 
    if (!(all_pieces & push_target)) {
      pawn_moves |= push_target;
    }
    // double-push task this out of the one-push if.
    if ((is_white && (RANK_2 & BIT(pawn_square))) ||
      (!is_white && (RANK_7 & BIT(pawn_square)))) {
      U64 double_push_target = is_white ? BIT(pawn_square) << 16 : BIT(pawn_square) >> 16;
      if (!(all_pieces & double_push_target)) {
        pawn_moves |= double_push_target;
      }
    } 

    // captures
    U64 right_capture = is_white ? push_target << 1 : push_target >> 1;
    U64 left_capture = is_white ? push_target >> 1 : push_target << 1;
    if (!(all_pieces & left_capture) && !(FILE_A & BIT(pawn_square))) {
      pawn_moves |= left_capture;
    }
    if (!(all_pieces & right_capture) && !(FILE_H & BIT(pawn_square))) {
      pawn_moves |= right_capture;
    }
  }
  
  return pawn_moves;
}

void print_board(U64 bitboard) 
{
  printf("\n");
  for(int rank=7; rank >= 0; rank--) {
    printf("  %d ", rank + 1);
    for(int file=0; file < 8; file++) {
      int square = rank * 8 + file;
      printf(" %d", get_bit(bitboard, square) ? 1 : 0);
    }
    printf("\n");
  }
  printf("\n     a b c d e f g h\n\n");
  printf("Bitboard: %llud\n\n", bitboard);
}

int main() 
{
  printf("U64 board\n");
  Board board;
  memset(&board, 0, sizeof(Board));

  set_bit(board.whitePawns, a2);
  set_bit(board.whitePawns, b2);
  set_bit(board.whitePawns, h2);
  set_bit(board.blackPawns, a7);
  set_bit(board.blackPawns, b4);
  set_bit(board.blackPawns, b7);
  init_board(&board);
  U64 pawn_moves = generate_pawn_moves(&board, white);
  
  print_board(board.whitePawns);
  print_board(pawn_moves);
  //print_board(board.blackPawns);
  return 0;
}









