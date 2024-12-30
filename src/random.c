#include <time.h>

#include "random.h"

uint64_t xorshift64_state = 2304978537;

uint64_t xorshift64() {
  uint64_t x = xorshift64_state;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  return xorshift64_state = x;
}

void init_random() {
  xorshift64_state = time(NULL);
}

uint64_t random_64bit() {
  return xorshift64() & xorshift64() & xorshift64() & xorshift64();
}
