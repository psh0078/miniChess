#ifndef RANDOM_H
#define RANDOM_H

#include "bitboard.h"

uint64_t xorshift64();
void init_random();
uint64_t random_64bit();

#endif
