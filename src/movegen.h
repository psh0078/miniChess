#include "bitboard.h"

// add all the possible moves from a square to a MoveList
void add_move(MoveList* moves, Square source, Square target);
void insert_king_moves(MoveList* moves, Bitboard allies);
