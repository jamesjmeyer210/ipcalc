#include <criterion/criterion.h>
#include "../src/lib/ipv4addr.h"

Test(ipv4_tests, try_uint32_to_ipv4_returns_ok)
{
  char* ipv4 = malloc(15);

  cr_assert(OK == try_uint32_to_ipv4(0, ipv4));
  cr_assert_str_eq("0.0.0.0", ipv4);

  cr_assert(OK == try_uint32_to_ipv4(16843263, ipv4));
  cr_assert_str_eq("1.1.1.255", ipv4);

  free(ipv4);
}

Test(ipv4_tests, try_uint32_to_ipv4_returns_error)
{
  char* ipv4 = NULL;

  cr_assert(ERR_ARG_NULL == try_uint32_to_ipv4(0, ipv4));
}

Test(ipv4_tests, ipv4_str_from_str_test_returns_error)
{
  Ipv4StrResult r = ipv4_str_from_str(NULL);
  cr_assert(r.status == ERR_ARG_NULL);

  const char* ipv4 = "lorem ipsum";
  r = ipv4_str_from_str(ipv4);
  cr_assert(r.status == ERR_ARG_INVALID);
}

Test(ipv4_tests, ipv4_str_from_str_test_returns_ok)
{
  const char* a = "127.0.0.1";
  Ipv4StrResult r = ipv4_str_from_str(a);

  cr_assert(OK == r.status);

  Ipv4Str ipv4 = r.value;
  cr_assert(string == ipv4.format);

  const char* b = "8888";
  r = ipv4_str_from_str(b);

  cr_assert(OK == r.status);
  cr_assert(decimal == r.value.format);
}

Test(ipv4_tests, try_ipv4_to_int32_returns_ok)
{
  Ipv4Str ipv4 = { .format = string, .str = NULL };
  uint32_t x;

  ipv4.str = "0.0.0.0";
  cr_assert(OK == try_ipv4_to_uint32(&ipv4, &x));
  cr_assert(0 == x);

  ipv4.str = "0.0.0.1";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (1 == x));

  ipv4.str = "0.0.0.255";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (255 == x));

  ipv4.str = "0.0.1.255";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (511 == x));

  ipv4.str = "0.1.1.255";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (66047 == x));

  ipv4.str = "1.1.1.255";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (16843263 == x));

  ipv4.str = "1.1.1.1";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (16843009 == x));

  ipv4.str = "127.0.0.1";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (2130706433 == x));

  ipv4.str = "198.168.0.1";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (3332898817 == x));

  ipv4.str = "255.255.255.255";
  cr_assert((OK == try_ipv4_to_uint32(&ipv4, &x)) && (4294967295 == x));
}