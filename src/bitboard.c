#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "bitboard.h"

void print_bitboard(Bitboard bitboard)
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


Bitboard init_all_pieces(Board* board)
{
  board->allWhitePieces = board->whitePawns | board->whiteRooks |
                          board->whiteKnights | board->whiteBishops |
                          board->whiteQueens | board->whiteKing;
  board->allBlackPieces = board->blackPawns | board->blackRooks |
                          board->blackKnights | board->blackBishops |
                          board->blackQueens | board->blackKing;
  return board->allPieces = board->allWhitePieces | board->allBlackPieces;
}

const int knight_offset[8] = {
  -17, -15, -10, -6, 6, 10, 15, 17
};

Bitboard gen_king_moves(Board* board, int side) // TODO: refactor this by using shift().
{
  bool is_white = (side == 0);
  Bitboard king_moves = 0ULL;
  Bitboard king = is_white ? board->whiteKing : board->blackKing;
  Bitboard king_sq;
  POP_LSB(king_sq, king);
  if (BIT(king_sq) >> 8) king_moves |= (BIT(king_sq) >> 8);
  if ((BIT(king_sq) >> 9) && !(FILE_A & BIT(king_sq)) && !(RANK_1 & BIT(king_sq))) king_moves |= (BIT(king_sq) >> 9);
  if ((BIT(king_sq) >> 7) && !(FILE_H & BIT(king_sq)) && !(RANK_1 & BIT(king_sq))) king_moves |= (BIT(king_sq) >> 7);
  if ((BIT(king_sq) >> 1) && !(FILE_A & BIT(king_sq))) king_moves |= (BIT(king_sq) >> 1);
  if (BIT(king_sq) << 8) king_moves |= (BIT(king_sq) << 8);
  if ((BIT(king_sq) << 9) && !(FILE_H & BIT(king_sq)) && !(RANK_8 & BIT(king_sq))) king_moves |= (BIT(king_sq) << 9);
  if ((BIT(king_sq) << 7) && !(FILE_A & BIT(king_sq)) && !(RANK_8 & BIT(king_sq))) king_moves |= (BIT(king_sq) << 7);
  if ((BIT(king_sq) << 1) && !(FILE_H & BIT(king_sq))) king_moves |= (BIT(king_sq) << 1);

  return king_moves;
}

Bitboard gen_knight_moves(Board* board, int side)
{
  bool is_white = (side == 0);
  Bitboard knight_moves = 0ULL;
  Bitboard knights = is_white ? board->whiteKnights : board->blackKnights;
  Bitboard all_pieces = init_all_pieces(board); // not being used..check if a square is occupied.
  Bitboard knight_sq;
  while (knights) {
    POP_LSB(knight_sq, knights);

    for (int i=0; i < 8; i++) {
      int target = knight_sq + knight_offset[i];
      if (target >= 0 && target < 64 &&
          abs((target % 8) - ((int) knight_sq % 8)) <= 2) {
        knight_moves |= BIT(target);
      }
    }
  }
  return knight_moves;
}

Bitboard gen_pawn_moves(Board* board, int side)
{
  bool is_white = (side == 0);
  Bitboard pawn_moves = 0ULL;
  Bitboard pawns = is_white ? board->whitePawns : board->blackPawns;
  Bitboard all_pieces = init_all_pieces(board);
  Bitboard pawn_square;
  // push moves
  while (pawns) {
    POP_LSB(pawn_square, pawns);

    // one-push
    Bitboard push_target = is_white ? shift(NORTH, BIT(pawn_square)) : shift(SOUTH, BIT(pawn_square));
    if (!(all_pieces & push_target)) {
      pawn_moves |= push_target;
    }
    // double-push
    if ((is_white && (RANK_2 & BIT(pawn_square))) ||
      (!is_white && (RANK_7 & BIT(pawn_square)))) {
      Bitboard double_push_target = is_white ? shift(NORTH2, BIT(pawn_square)) : shift(SOUTH2, BIT(pawn_square));
      if (!(all_pieces & double_push_target)) {
        pawn_moves |= double_push_target;
      }
    }

    // captures
    Bitboard right_capture = is_white ? shift(EAST, push_target) : shift(WEST, push_target);
    Bitboard left_capture = is_white ? shift(WEST, push_target) : shift(EAST, push_target);
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

void expand_moves(Bitboard all_moves, Bitboard moves[64]) {
    for (int i = 0; i < 64; i++) {
        if (all_moves & (1ULL << i)) {
            moves[i] = all_moves & ~(1ULL << i);
        } else {
            moves[i] = 0;
        }
    }
}

void init_leapers_attacks(Board* board)
{
  white_pawn_moves = gen_pawn_moves(board, white);
  expand_moves(white_pawn_moves, white_pawn_attacks);

  black_pawn_moves = gen_pawn_moves(board, black);
  expand_moves(black_pawn_moves, black_pawn_attacks);

  knight_moves = gen_knight_moves(board, white);
  expand_moves(knight_moves, knight_attacks);

  king_moves = gen_king_moves(board, white);
  expand_moves(king_moves, king_attacks);
}

int main()
{
  printf("Bitboard board\n");
  Board board;
  memset(&board, 0, sizeof(Board));

  set_bit(board.whitePawns, a2);
  set_bit(board.whiteKnights, g2);
  set_bit(board.whiteKing, d8);

  // init_leapers_attacks(&board ,white);
  // print_bitboard(knight_moves);
  // pawn_moves = gen_pawn_moves(&board, white);
  // print_bitboard(pawn_moves);
  // expand_moves(pawn_moves, white_pawn_attacks);
  king_moves = gen_king_moves(&board, white);
  print_bitboard(king_moves);
  //print_bitboard(board.whitePawns);
  //print_bitboard(pawn_moves);
  //print_bitboard(board.blackPawns);
  return 0;
}
