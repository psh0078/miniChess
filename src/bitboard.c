#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "bitboard.h"

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
    U64 push_target = is_white ? shift(NORTH, BIT(pawn_square)) : shift(SOUTH, BIT(pawn_square)); 
    if (!(all_pieces & push_target)) {
      pawn_moves |= push_target;
    }
    // double-push task this out of the one-push if.
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
  init_board(&board);
  U64 pawn_moves = generate_pawn_moves(&board, white);
  
  print_board(board.whitePawns);
  print_board(pawn_moves);
  //print_board(board.blackPawns);
  return 0;
}









