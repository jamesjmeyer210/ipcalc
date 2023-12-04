#include <criterion/criterion.h>
#include "../src/lib/convert.h"

Test(convert_tests, try_uint8_from_str_returns_ok)
{
	uint8_t x = 0;
	cr_assert(OK == try_uint8_from_str("0", &x) && 0 == x);
	cr_assert(OK == try_uint8_from_str("9", &x) && 9 == x);
	cr_assert(OK == try_uint8_from_str("88", &x) && 88 == x);
	cr_assert(OK == try_uint8_from_str("255", &x) && 255 == x);
}

Test(convert_tests, try_uint8_from_str_returns_ERROR_ARG_NULL)
{
	uint8_t x = 0;
	cr_assert(ERR_ARG_NULL == try_uint8_from_str(NULL, &x));
	cr_assert(ERR_ARG_NULL == try_uint8_from_str("1", NULL));
	cr_assert(ERR_ARG_NULL == try_uint8_from_str(NULL, NULL));
}


Test(convert_tests, try_uint8_from_str_returns_ERR_ARG_INVALID)
{
	uint8_t x = 0;
	cr_assert(ERR_ARG_INVALID == try_uint8_from_str("-1", &x));
  cr_assert(ERR_ARG_INVALID == try_uint8_from_str("foo", &x));
}