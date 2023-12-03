#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/test.h"
#include "../src/lib/string.h"
#include "../src/ipv4addr.h"
#include "../src/lib/convert.h"

typedef struct test {
  char* name;
  void (*func)(void);
} Test;

typedef struct test_collection {
  Test* n;
  size_t count;
} TestCollection;

static void str_split_test()
{
  const char* ipv4 = "198.162.0.1";
  Strings spaces = str_split(ipv4, '.');
  printf("%s\n", spaces.data[0]);
  printf("%s\n", spaces.data[1]);
  printf("%s\n", spaces.data[2]);
  printf("%s\n", spaces.data[3]);

  ASSERT(spaces.count == 4)
  ASSERT(0 == strcmp("198", spaces.data[0]))
  ASSERT(0 == strcmp("162", spaces.data[1]))
  ASSERT(0 == strcmp("0", spaces.data[2]))
  ASSERT(0 == strcmp("1", spaces.data[3]))

  strings_free(&spaces);
}

static void str_nsubstr_test_in_bounds_1()
{
  const char* src = "Hello world.";
  char dest[10];

  const char* x = str_nsubstr(src, 0, 5, dest, 10);
  ASSERT(x != NULL)
  ASSERT(str_eq("Hello", x))
}

static void str_nsubstr_test_in_bounds_2()
{
  const char* src = "Hello world.";
  char dest[10];

  const char* x = str_nsubstr(src, 6, 12, dest, 10);
  ASSERT(x != NULL)
  ASSERT(str_eq("world.", x))
}

static void str_nsubstr_test_out_of_bounds_1()
{
  const char* src = "Hello world.";
  char dest[10];

  const char* x = str_nsubstr(src, 0, 12, dest, 10);
  ASSERT(x != NULL)
  ASSERT_EQ(size_t, (size_t)10, strlen(x))
  ASSERT(str_eq("Hello worl", x))
}

static void str_nsubstr_test_out_of_bounds_2()
{
  const char* src = "Hello world.";
  char dest[10];

  const char* x = str_nsubstr(src, 5, 20, dest, 10);
  ASSERT(x != NULL)
  ASSERT_EQ(size_t, (size_t)7, strlen(x))
  ASSERT(str_eq(" world.", x))
}

static void str_nsplit_test()
{
  char* src = "127.0.0.1";
  char copy[15] = {'\0'};
  /*array(char) x = array_init(char)(malloc(sizeof(char*) * 4), 4);

  array(char)* y = str_nsplit(src, '.', copy, &x);

  ASSERT(y == &x);
  ASSERT(str_eq("127", x.data[0]));
  ASSERT(str_eq("0", x.data[1]));
  ASSERT(str_eq("0", x.data[2]));
  ASSERT(str_eq("1", x.data[3]));

  array_free(char)(y);*/
}

static void string_tests()
{
  str_split_test();
  str_nsubstr_test_in_bounds_1();
  str_nsubstr_test_in_bounds_2();
  str_nsubstr_test_out_of_bounds_1();
  str_nsubstr_test_out_of_bounds_2();
  str_nsplit_test();
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

int main(int argc, char** argv)
{
  printf("%d\n", argc);
  if (argc == 1)
  {
    try_ipv4_to_int32_test();
    string_tests();
    try_uint8_from_str_test();
    exit(0);
  }

  TestCollection tests = {0};
  tests.n = malloc(sizeof(Test) * 100);
  Test t = { .name = "str_split_test", .func = &str_split_test };
  tests.n[0] = t;
  tests.count++;

  for(int i = 1; i < argc; i++)
  {
    for(size_t j = 0; j < tests.count; j++)
    {
      if(str_eq(argv[i], tests.n[j].name))
      {
        tests.n[j].func();
      }
    }
  }

  exit(0);
}