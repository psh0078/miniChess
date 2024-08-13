#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "bitboard.h"

U64 pawn_attacks[2][64];

U64 init_all_pieces(Board* board)
{
  board->allWhitePieces = board->whitePawns | board->whiteRooks |
                          board->whiteKnights | board->whiteBishops |
                          board->whiteQueens | board->whiteKing;
  board->allBlackPieces = board->blackPawns | board->blackRooks |
                          board->blackKnights | board->blackBishops |
                          board->blackQueens | board->blackKing;
  return board->allPieces = board->allWhitePieces | board->allBlackPieces; 
}

void init_leapers_attacks()
{
 
}

const int knight_offset[8] = {
  -17, -15, -10, -6, 6, 10, 15, 17
};

U64 gen_knight_moves(Board* board, int side)
{
  bool is_white = (side == 0);
  U64 knight_moves = 0ULL;
  U64 knights = is_white ? board->whiteKnights : board->blackKnights; 
  U64 all_pieces = init_all_pieces(board);
  U64 knight_square;
  while (knights) {
    POP_LSB(knight_square, knights);
    
    for (int i=0; i < 8; i++) {
      int target = knight_square + knight_offset[i];
      if (target >= 0 && target < 64 ) {
        printf("%llud\n", BIT(target));
        knight_moves |= BIT(target); 
      }
    }
  }
  return knight_moves;
}

U64 gen_pawn_moves(Board* board, int side)
{
  bool is_white = (side == 0);
  U64 pawn_moves = 0ULL;
  U64 pawns = is_white ? board->whitePawns : board->blackPawns; 
  U64 all_pieces = init_all_pieces(board);
  U64 pawn_square;
  // push moves
  while (pawns) {
    POP_LSB(pawn_square, pawns);

    // one-push
    U64 push_target = is_white ? shift(NORTH, BIT(pawn_square)) : shift(SOUTH, BIT(pawn_square)); 
    if (!(all_pieces & push_target)) {
      pawn_moves |= push_target;
    }
    // double-push
    if ((is_white && (RANK_2 & BIT(pawn_square))) ||
      (!is_white && (RANK_7 & BIT(pawn_square)))) {
      U64 double_push_target = is_white ? shift(NORTH2, BIT(pawn_square)) : shift(SOUTH2, BIT(pawn_square));
      if (!(all_pieces & double_push_target)) {
        pawn_moves |= double_push_target;
      }
    } 

    // captures
    U64 right_capture = is_white ? shift(EAST, push_target) : shift(WEST, push_target);
    U64 left_capture = is_white ? shift(WEST, push_target) : shift(EAST, push_target);
    if (!(all_pieces & left_capture) && !(FILE_A & BIT(pawn_square))) {
      pawn_moves |= left_capture;
    }
    if (!(all_pieces & right_capture) && !(FILE_H & BIT(pawn_square))) {
      pawn_moves |= right_capture;
    }

    // en passant

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
  set_bit(board.blackPawns, b7);
  U64 pawn_moves = gen_pawn_moves(&board, white);
  
  set_bit(board.whiteKnights, b2);
  U64 knight_moves = gen_knight_moves(&board, white);
  print_board(knight_moves);

  //print_board(board.whitePawns);
  //print_board(pawn_moves);
  //print_board(board.blackPawns);
  return 0;
}









