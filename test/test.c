#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/bits.h"
#include "../src/lib/string.h"
#include "../src/ipv4addr.h"
#include "../src/lib/convert.h"

#define TEST 1

static int passes = 0;
static int failures = 0;

#define ASSERT(expr)\
if(expr)\
{\
  printf("%s: line: %d: %s PASS\n", __FUNCTION__, __LINE__, #expr); \
  passes++;\
}\
else{\
  printf("%s: line: %d: %s FAIL\n", __FUNCTION__, __LINE__, #expr); \
  failures++;\
}

static void str_split_test()
{
    const char* ipv4 = "198.162.0.1";
    List* spaces = str_split(ipv4, '.');
    ASSERT(spaces->len == 4)
    ASSERT(0 == strcmp("198", (char*)spaces->data[0]))
    ASSERT(0 == strcmp("162", (char*)spaces->data[1]))
    ASSERT(0 == strcmp("0", (char*)spaces->data[2]))
    ASSERT(0 == strcmp("1", (char*)spaces->data[3]))

    list_free(spaces);
}

static void string_tests()
{
    str_split_test();
}

static void try_ipv4_to_int32_test()
{
  uint32_t x;
  ASSERT((OK == try_ipv4_to_uint32("0.0.0.0", &x)) && (0 == x))
  ASSERT((OK == try_ipv4_to_uint32("0.0.0.1", &x)) && (1 == x))
  ASSERT((OK == try_ipv4_to_uint32("0.0.0.255", &x)) && (255 == x))
  ASSERT((OK == try_ipv4_to_uint32("0.0.1.255", &x)) && (511 == x))
  ASSERT((OK == try_ipv4_to_uint32("0.1.1.255", &x)) && (66047 == x))
  ASSERT((OK == try_ipv4_to_uint32("1.1.1.255", &x)) && (16843263 == x))
  ASSERT((OK == try_ipv4_to_uint32("1.1.1.1", &x)) && (16843009 == x))
  ASSERT((OK == try_ipv4_to_uint32("127.0.0.1", &x)) && (2130706433 == x))
  ASSERT((OK == try_ipv4_to_uint32("198.168.0.1", &x)) && (3332898817 == x))
  ASSERT((OK == try_ipv4_to_uint32("255.255.255.255", &x)) && (4294967295 == x))
}

static void try_uint8_from_str_test()
{
  uint8_t x;
  ASSERT(OK == try_uint8_from_str("0", &x) && (0 == x))
  ASSERT(OK == try_uint8_from_str("9", &x) && (9 == x))
  ASSERT(OK == try_uint8_from_str("88", &x) && (88 == x))
  ASSERT(OK == try_uint8_from_str("255", &x) && (255 == x))
  /*ASSERT(4294967295 == uint32_from_str("4294967295"))
  ASSERT(0 == uint32_from_str("4294967296"))*/
}

int main(void)
{
  try_ipv4_to_int32_test();
  string_tests();
  try_uint8_from_str_test();
  exit(0);
}