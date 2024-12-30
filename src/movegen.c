#include "movegen.h"

void add_move(MoveList* moves, Square source, Square target) {
  moves->moves[moves->size].source = source;
  moves->moves[moves->size].target = target;
  moves->size++;
}
