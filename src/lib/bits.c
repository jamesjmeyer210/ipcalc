#include "bits.h"

uint8_t count_bits(uint32_t x)
{
  if(x == 0) return 0;

  uint32_t y = x;
  uint8_t bits = 0;
  for(; y != 0; bits++)
  {
    y = y >> 1;
  }

  return bits;
}