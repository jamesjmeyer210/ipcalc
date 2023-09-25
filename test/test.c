#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/string.h"
#include "../src/ipv4addr.h"
#include "../src/lib/convert.h"

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

static void ipv4addr_tests()
{
  ASSERT(0 == ipv4_space_to_byte("0"))
  ASSERT(9 == ipv4_space_to_byte("9"))
  ASSERT(11 == ipv4_space_to_byte("11"))
  ASSERT(222 == ipv4_space_to_byte("222"))
  ASSERT(2 == ipv4_space_to_byte("2"))

  ASSERT(0 == ipv4_to_int("0.0.0.0"))
  ASSERT(1 == ipv4_to_int("0.0.0.1"))
  ASSERT(255 == ipv4_to_int("0.0.0.255"))
  ASSERT(511 == ipv4_to_int("0.0.1.255"))
  ASSERT(66047 == ipv4_to_int("0.1.1.255"))
  ASSERT(16843263 == ipv4_to_int("1.1.1.255"))
  ASSERT(16843009 == ipv4_to_int("1.1.1.1"))
  ASSERT(2130706433 == ipv4_to_int("127.0.0.1"))
  ASSERT(3332898817 == ipv4_to_int("198.168.0.1"))
  ASSERT(4294967295 == ipv4_to_int("255.255.255.255"))
}

static void convert_tests()
{
  ASSERT(0 == uint32_from_str("0"))
  ASSERT(9 == uint32_from_str("9"))
  ASSERT(88 == uint32_from_str("88"))
  ASSERT(4294967295 == uint32_from_str("4294967295"))
  ASSERT(0 == uint32_from_str("4294967296"))
}

int main(void)
{
  convert_tests();
  string_tests();
  ipv4addr_tests();
  exit(0);
}