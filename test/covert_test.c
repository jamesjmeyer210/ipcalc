#include <criterion/criterion.h>
#include "../src/lib/result.h"
#include "../src/lib/convert.h"

Test(convert_tests, u8_from_str_returns_ok)
{
	r_u8 x;
  x = u8_from_str("0");
	cr_assert(OK == x.status && 0 == x.value);

  x = u8_from_str("9");
  cr_assert(OK == x.status && 9 == x.value);

  x = u8_from_str("88");
  cr_assert(OK == x.status && 88 == x.value);

  x = u8_from_str("255");
  cr_assert(OK == x.status && 255 == x.value);
}

Test(convert_tests, u8_from_str_returns_ERR_ARG_OUT_OF_RANGE)
{
	r_u8 x;
	x = u8_from_str("256");
  cr_assert(ERR_ARG_OUT_OF_RANGE == x.status);
}


Test(convert_tests, try_uint8_from_str_returns_ERR_ARG_INVALID)
{
  r_u8 x;
  x = u8_from_str("-1");
	cr_assert(ERR_ARG_INVALID == x.status);

  x = u8_from_str("foo");
  cr_assert(ERR_ARG_INVALID == x.status);
}