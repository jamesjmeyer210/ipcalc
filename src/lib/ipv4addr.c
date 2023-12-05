#include "ipv4regex.h"
#include "ipv4addr.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "error.h"
#include "string.h"
#include "convert.h"
#include "bits.h"

static uint8_t from_char(const char c)
{
  uint8_t x = (uint8_t)c;
  return x - 48;
}

static uint8_t ipv4_space_to_byte(const char* ipv4_space)
{
  uint8_t x = 0;
  switch(strlen(ipv4_space))
  {
    case 1:
      x = from_char(ipv4_space[0]);
      break;
    case 2:
      {
        uint8_t x0 = from_char(ipv4_space[0]);
        uint8_t x1 = from_char(ipv4_space[1]);
        x = (10 * x0) + x1;
      }
      break;
    case 3:
      {
        uint8_t x0 = from_char(ipv4_space[0]);
        uint8_t x1 = from_char(ipv4_space[1]);
        uint8_t x2 = from_char(ipv4_space[2]);
        x = (100 * x0) + (10 * x1) + x2;
      }
      break;
    default:
      break;
  }
  return x;
}

error_t ipv4_str_from_str(const char* ipv4_str, Ipv4Str* result)
{
  if(ipv4_str == NULL) return ERR_MSG(ERR_ARG_NULL, "Argument %s is null", "ipv4_str");
  if(result == NULL) return ERR_MSG(ERR_ARG_NULL, "Argument %s is null", "result");

  regex_t regex = init_ipv4_regex();
  if(regex_is_valid(&regex, ipv4_str))
  {
    result->format = string;
    result->str = ipv4_str;
    return OK;
  }

  if(strn_is_numeric(ipv4_str, strlen(ipv4_str)))
  {
    result->format = decimal;
    result->str = ipv4_str;
    return OK;
  }

  return ERR_MSG(ERR_ARG_INVALID, "Could not identify a format for \"%s\"", ipv4_str);
}

error_t try_ipv4_to_uint32(const char* ipv4_str, uint32_t* result)
{
  assert(ipv4_str != NULL);
  assert(result != NULL);

  regex_t regex = init_ipv4_regex();
  if(!regex_is_valid(&regex, ipv4_str))
  {
    return ERR_MSG(ERR_ARG_INVALID, "IPV4 address \"%s\" is invalid\"\n", ipv4_str);
  }

  Strings spaces = str_split(ipv4_str, '.');
  if(spaces.count != 4)
  {
    strings_free(&spaces);
    return ERR_MSG(ERR_ARG_INVALID, "IPV4 address \"%s\" could not be parsed\n", ipv4_str);
  }

  uint8_t x1 = ipv4_space_to_byte(spaces.data[0]);
  uint8_t x2 = ipv4_space_to_byte(spaces.data[1]);
  uint8_t x3 = ipv4_space_to_byte(spaces.data[2]);
  uint8_t x4 = ipv4_space_to_byte(spaces.data[3]);

  strings_free(&spaces);
  *result = ((uint32_t)x1 << 24) + ((uint32_t)x2 << 16) + ((uint32_t)x3 << 8) + (uint32_t)x4;
  return OK;
}

error_t try_uint32_to_ipv4(uint32_t ipv4, char* result)
{
  if(result == NULL) return ERR(ERR_ARG_NULL);

  uint8_t x1 = ipv4 >> 24;
  uint8_t x2 = (ipv4 << 8) >> 24;
  uint8_t x3 = (ipv4 << 16) >> 24;
  uint8_t x4 = (ipv4 << 24) >> 24;

  snprintf(result, 15, "%d.%d.%d.%d", x1, x2, x3, x4);
  return OK;
}

error_t try_decimal_to_ipv4(const char* ipv4, char* result)
{
  if(ipv4 == NULL) return ERR(ERR_ARG_NULL);
  if(result == NULL) return ERR(ERR_ARG_NULL);

  uint32_t x;
  if(try_uint32_from_str(ipv4, &x) != OK)
  {
    return get_error_code();
  }
  if(x == 0 && !str_eq("0", ipv4))
  {
    return ERR(ERR_ARG_INVALID);
  }

  try_uint32_to_ipv4(x, result);
  return OK;
}

static error_t try_get_ipv4_colon_range(const regex_t* ipv4_regex, const char* str, Ipv4Range* result)
{
  Strings addrs = str_split(str, ':');
  if(addrs.len != 2)
  {
    strings_free(&addrs);
    return ERR_MSG(ERR_ARG_INVALID, "IPV4 address \"%s\" is missing the delimiter \':\'\n", str);
  }

  for(size_t i = 0; i < addrs.len; i++)
  {
    if(!regex_is_valid(ipv4_regex, addrs.data[i]))
    {
      strings_free(&addrs);
      return ERR_MSG(ERR_ARG_INVALID, "IPV4 address \"%s\" is invalid\n", (char*)addrs.data[i]);
    }
  }

  if(OK != try_ipv4_to_uint32(addrs.data[0], &result->lower)) return get_error_code();
  if(OK != try_ipv4_to_uint32(addrs.data[1], &result->upper)) return get_error_code();
  if(result->lower > result->upper)
  {
    return ERR_MSG(ERR_ARG_INVALID, "Lower value of IPV4 address \"%s\" is greater than upper value\n", str);
  }

  result->bits = count_bits(result->upper - result->lower);
  uint32_t diff = result->upper - result->lower;
  uint32_t limit = 1 << result->bits;
  uint32_t remainder = limit - diff - 1;
  result->remainder = remainder;
  if(result->remainder > 0)
  {
    result->bits--;
  }

  strings_free(&addrs);
  return OK;
}

static error_t try_get_ipv4_slash_range(const regex_t* regex, const char* str, Ipv4Range* result)
{
  Strings addrs = str_split(str, '/');
  if(addrs.len != 2)
  {
    strings_free(&addrs);
    return ERR_MSG(ERR_ARG_INVALID, "IPV4 address \"%s\" is missing the delimiter \'/\'\n", str);
  }

  if(!regex_is_valid(regex, addrs.data[0]))
  {
    strings_free(&addrs);
    return ERR_MSG(ERR_ARG_INVALID, "IPV4 address \"%s\" is invalid\n", (char*)addrs.data[0]);
  }

  if(OK != try_ipv4_to_uint32(addrs.data[0], &result->lower)) return get_error_code();
  if(OK != try_uint8_from_str(addrs.data[1], &result->bits)) return get_error_code();
  result->upper = result->lower + (1 << result->bits);
  strings_free(&addrs);
  return OK;
}

error_t try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result)
{
  assert(ipv4_regex != NULL);
  assert(str != NULL);
  assert(result != NULL);

  switch (format)
  {
    case COLON: return try_get_ipv4_colon_range(ipv4_regex, str, result);
    case SLASH: return try_get_ipv4_slash_range(ipv4_regex, str, result);
  }
}

Ipv4Range* ipv4_range_next(const Ipv4Range* self, Ipv4Range* next)
{
  if(self == NULL || next == NULL) return next;

  next->lower = self->lower + (1 << self->bits);
  next->upper = self->upper;
  next->bits = count_bits(next->upper - next->lower);
  uint32_t diff = next->upper - next->lower;
  uint32_t limit = 1 << next->bits;
  next->remainder = limit - diff - 1;
  if(next->remainder > 0)
  {
    next->bits--;
  }
  return next;
}
