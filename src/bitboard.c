#include <complex.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "bitboard.h"
#include "attacks.h"

const char* PIECE_TO_CHAR = "PpNnBbRrQqKk";
const int CHAR_TO_PIECE[] = {
  ['P'] = WHITE_PAWN,   //
  ['N'] = WHITE_KNIGHT, //
  ['B'] = WHITE_BISHOP, //
  ['R'] = WHITE_ROOK,   //
  ['Q'] = WHITE_QUEEN,  //
  ['K'] = WHITE_KING,   //
  ['p'] = BLACK_PAWN,   //
  ['n'] = BLACK_KNIGHT, //
  ['b'] = BLACK_BISHOP, //
  ['r'] = BLACK_ROOK,   //
  ['q'] = BLACK_QUEEN,  //
  ['k'] = BLACK_KING,   //
};

bool get_bit(Bitboard bitboard, int square) {
  return (bitboard & BIT(square)) != 0;
}

void set_bit(Bitboard* bitboard, int square) {
  *bitboard |= BIT(square);
}

void clear_bit(Bitboard* bitboard, int square) {
  if (get_bit(*bitboard, square)) {
    *bitboard &= ~BIT(square);
  }
}

int pop_1st_bit(Bitboard* bb) {
  if (*bb == 0) return -1;
  int index = __builtin_ctzll(*bb);
  *bb &= (*bb - 1);
  return index;
}
Bitboard shift(enum Direction D, Bitboard b) {
  return D == NORTH         ? b << 8
       : D == SOUTH         ? b >> 8
       : D == EAST          ? (b & ~FILE_H) << 1
       : D == WEST          ? (b & ~FILE_A) >> 1
       : D == NORTH_EAST    ? (b & ~FILE_H) << 9
       : D == NORTH_WEST    ? (b & ~FILE_A) << 7
       : D == SOUTH_EAST    ? (b & ~FILE_H) >> 7
       : D == SOUTH_WEST    ? (b & ~FILE_A) >> 9
       : 0;
}

void clear_board(Board* board) {
  memset(board->pieces, 0, sizeof(board->pieces));
  memset(board->occupancies, 0, sizeof(board->occupancies));
  
  for (int i = 0; i < 64; i++)
    board->squares[i] = NO_PIECE;
  
  board->stm       = white;
  board->xstm      = black;
  board->enpassant = 0;
  board->castling  = 0;
}

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

void print_board(Board* board) {
  for (int r = 0; r < 8; r++) {
    printf("+-------+-------+-------+-------+-------+-------+-------+-------+\n");
    printf("|");
    for (int f = 0; f < 16; f++) {
      if (f == 8)
        printf("\n|");

      int sq = r * 8 + (f > 7 ? f - 8 : f);

      if (f < 8) {
        if (board->squares[sq] == NO_PIECE)
          printf("       |");
        else
          printf("   %c   |", PIECE_TO_CHAR[board->squares[sq]]);
      } else {
        printf("       |");
      }
    }
    printf("\n");
  }
  printf("+-------+-------+-------+-------+-------+-------+-------+-------+\n");
}

void parse_fen(char* fen, Board* board) {
  clear_board(board);
  for (int i = 0; i < 64; i++) {
    if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
      int piece = CHAR_TO_PIECE[(int) *fen];
      set_bit(&board->pieces[piece], i);
      board->squares[i] = piece;
      fen++;
    }
    if (*fen >= '0' && *fen <= '9') {
      int offset = *fen - '1';
      i += offset
    } else if (*fen == '/') {
      i--;
    }

    fen++;
  }
  fen ++;

  board->side = (*fen++ == 'w' ? white : black);
}

int main() {
  printf("Bitboard board\n");
  Board board;
  clear_board(&board);
  initMagics();
  init_sliders_attacks(1);
  init_sliders_attacks(0);
  init_leapers_attacks();
  print_board(&board);

  //Bitboard pawn_occ = 0ULL;
  //set_bit(&pawn_occ, d3);
  //set_bit(&pawn_occ, e4);
  //print_bitboard(pawn_occ);
  //print_bitboard(PAWN_ATTACKS[black][a3]);

  // Bitboard bishop_occupancy = 0ULL;
  // set_bit(&bishop_occupancy, g7);
  // set_bit(&bishop_occupancy, f6);
  // set_bit(&bishop_occupancy, c5); 
  // set_bit(&bishop_occupancy, b2);
  // set_bit(&bishop_occupancy, g1);
  // printf("\n     Bishop occupancy\n");
  // print_bitboard(bishop_occupancy);
  // printf("\n     Bishop attacks\n");
  // print_bitboard(get_bishop_attacks(d4, bishop_occupancy));

  //Bitboard queen_occ = 0ULL;
  //set_bit(&queen_occ, d5);
  //print_bitboard(queen_occ);
  //print_bitboard(get_queen_attacks(d4, queen_occ));

  return 0;
}
