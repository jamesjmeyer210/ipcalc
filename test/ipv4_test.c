#include <criterion/criterion.h>
#include "../src/lib/ipv4addr.h"

Test(ipv4_tests, try_ipv4_to_int32_returns_ok)
{
  uint32_t x;
  cr_assert(OK == try_ipv4_to_uint32("0.0.0.0", &x));
  cr_assert(0 == x);

  cr_assert((OK == try_ipv4_to_uint32("0.0.0.1", &x)) && (1 == x));
  cr_assert((OK == try_ipv4_to_uint32("0.0.0.255", &x)) && (255 == x));
  cr_assert((OK == try_ipv4_to_uint32("0.0.1.255", &x)) && (511 == x));
  cr_assert((OK == try_ipv4_to_uint32("0.1.1.255", &x)) && (66047 == x));
  cr_assert((OK == try_ipv4_to_uint32("1.1.1.255", &x)) && (16843263 == x));
  cr_assert((OK == try_ipv4_to_uint32("1.1.1.1", &x)) && (16843009 == x));
  cr_assert((OK == try_ipv4_to_uint32("127.0.0.1", &x)) && (2130706433 == x));
  cr_assert((OK == try_ipv4_to_uint32("198.168.0.1", &x)) && (3332898817 == x));
  cr_assert((OK == try_ipv4_to_uint32("255.255.255.255", &x)) && (4294967295 == x));
}

Test(ipv4_tests, try_ipv4_to_int32_returns_error)
{
  uint32_t x = 0;
  cr_assert(ERR_ARG_INVALID == try_ipv4_to_uint32("0.0.0", &x));
  cr_assert(0 == x);
}

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

Test(ipv4_tests, ipv4_str_from_str_test)
{
  char* ipv4 = NULL;

  Ipv4StrResult r = ipv4_str_from_str(ipv4);
  cr_assert(ERR_ARG_NULL == r.status);
}
