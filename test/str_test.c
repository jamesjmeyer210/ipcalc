#include <string.h>
#include "../src/lib/string.h"
#include "../src/lib/ipv4addr.h"
#include "criterion/criterion.h"

Test(str_tests, str_eq_test)
{
  const char* a = "foo";
  const char* b = "foo";

  cr_assert_str_eq(a, b);
  cr_assert(true == str_eq(a, b), "str_eq(%s, %s) returns true", a, b);
}

Test(str_tests, str_split_test)
{
  const char* ipv4 = "198.162.0.1";
  Strings spaces = str_split(ipv4, '.');

  cr_assert(spaces.count == 4, "spaces.count == %d", 4);

  cr_expect(spaces.data[0] != NULL);
  cr_assert_str_eq("198", spaces.data[0]);

  cr_expect(spaces.data[1] != NULL);
  cr_assert_str_eq("162", spaces.data[1]);

  cr_expect(spaces.data[2] != NULL);
  cr_assert_str_eq("0", spaces.data[2]);

  cr_expect(spaces.data[3] != NULL);
  cr_assert_str_eq("1", spaces.data[3]);

  strings_free(&spaces);
  cr_expect(spaces.count == 0);
  cr_expect(spaces.len == 0);
}

Test(str_tests, strn_is_numeric_returns_true)
{
  const char* n = "100";

  cr_assert(strn_is_numeric(n, 3));
}

Test(str_tests, strn_is_numeric_returns_false_for_negative)
{
  const char* n = "-100";

  cr_assert(!strn_is_numeric(n, 4));
}

Test(str_tests, strn_is_numeric_returns_false)
{
  char* n = "foo";

  cr_assert(!strn_is_numeric(n, 3));

  n = "!100";
  cr_assert(!strn_is_numeric(n, 4));
}
