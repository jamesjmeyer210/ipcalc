#include "bits.h"

uint8_t count_bits(uint32_t x)
{
  if(x == 0) return 0;

  uint8_t bits = 0;
  for(uint32_t i = x; i != 0; bits++)
  {
    i = i >> 1;
  }

  return bits;
}


#ifdef TEST

#include "assert.h"
#include "stdio.h"

void count_bits_test()
{
  assert(0 == count_bits(0)); // 0000
  assert(1 == count_bits(1)); // 0001
  assert(2 == count_bits(2)); // 0010
  assert(2 == count_bits(3)); // 0011
  assert(3 == count_bits(4)); // 0100
  assert(3 == count_bits(5)); // 0101
  assert(3 == count_bits(6)); // 0110
  assert(3 == count_bits(7)); // 0111
  assert(4 == count_bits(8)); // 1000
  assert(4 == count_bits(15)); // 1111
  assert(5 == count_bits(16)); // 0001 0000
  assert(5 == count_bits(31)); // 0001 1111
}

int main(void)
{
  printf("Running bits-test\n");
  count_bits_test();
  return 0;
}
#endif