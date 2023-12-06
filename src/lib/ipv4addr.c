#include "ipv4regex.h"
#include "ipv4addr.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "error.h"
#include "string.h"
#include "convert.h"
#include "bits.h"

IMPL_RESULT(Ipv4Str, Ipv4StrResult)

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

Ipv4StrResult ipv4_str_from_str(const char* ipv4_str)
{
  if(NULL == ipv4_str){
    return Ipv4StrResult_err(ERR_MSG(ERR_ARG_NULL, "Argument \"%s\" must not be null", "ipv4_str"));
  }

  regex_t regex = init_ipv4_regex();
  if(regex_is_valid(&regex, ipv4_str)){
    Ipv4Str x = { .format = string, .str = ipv4_str };
    return Ipv4StrResult_ok(x);
  }

  if(strn_is_numeric(ipv4_str, strlen(ipv4_str)))
  {
    Ipv4Str x = { .format = decimal, .str = ipv4_str };
    return Ipv4StrResult_ok(x);
  }

  return Ipv4StrResult_err(ERR_MSG(ERR_ARG_INVALID, "Could not identify a format for \"%s\"", ipv4_str));
}

r_u32 ipv4_str_to_u32(const Ipv4Str* ipv4_str)
{
  if(NULL == ipv4_str) return r_u32_err(ERR_MSG(ERR_ARG_NULL, "Argument \"%s\" must not be null", "ipv4_str"));

  if(ipv4_str->format != string)
  {
    return r_u32_err(ERR_MSG(ERR_ARG_INVALID,
       "\"%s\" is not a valid format for converting to a 32-bit integer",
       ipv4_str->str));
  }

  Strings spaces = str_split(ipv4_str->str, '.');
  uint8_t x1 = ipv4_space_to_byte(spaces.data[0]);
  uint8_t x2 = ipv4_space_to_byte(spaces.data[1]);
  uint8_t x3 = ipv4_space_to_byte(spaces.data[2]);
  uint8_t x4 = ipv4_space_to_byte(spaces.data[3]);

  strings_free(&spaces);
  u32 x = ((uint32_t)x1 << 24) + ((uint32_t)x2 << 16) + ((uint32_t)x3 << 8) + (uint32_t)x4;
  return r_u32_ok(x);
}

error_t try_uint32_to_ipv4(uint32_t ipv4, char* result)
{
  if(NULL == result) return ERR_MSG(ERR_ARG_NULL, "Argument \"%s\" must not be null", "result");
  //ERR_IF_NULL(result, "result");

  uint8_t x1 = ipv4 >> 24;
  uint8_t x2 = (ipv4 << 8) >> 24;
  uint8_t x3 = (ipv4 << 16) >> 24;
  uint8_t x4 = (ipv4 << 24) >> 24;

  snprintf(result, 15, "%d.%d.%d.%d", x1, x2, x3, x4);
  return OK;
}

r_str decimal_to_ipv4(const Ipv4Str* ipv4)
{
  if(ipv4 == NULL) return r_str_err(ERR_MSG(ERR_ARG_NULL, "Argument %s must not be null", "ipv4"));

  r_u32 x = u32_from_str(ipv4);
  if(x.status != OK)
    return r_str_err(x.status);

  if(x.value == 0 && !str_eq("0", ipv4->str))
  {
    return r_str_err(ERR_MSG(ERR_ARG_INVALID, "Invalid IPv4 address: %s", ipv4->str));
  }

  char* value = malloc(15);
  try_uint32_to_ipv4(x.status, value);
  return r_str_ok(value);
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

  Ipv4Str ipv4[2];
  for(int i = 0; i < 2; i ++){
    Ipv4StrResult r_ipv4 = ipv4_str_from_str(addrs.data[0]);
    if(r_ipv4.status != OK) {
      return r_ipv4.status;
    }
    ipv4[i] = r_ipv4.value;
  }

  r_u32 r = ipv4_str_to_u32(&ipv4[0]);
  if(OK != r.status){
    return r.status;
  }
  result->lower = r.value;

  r = ipv4_str_to_u32(&ipv4[1]);
  if(OK != r.status){
    return r.status;
  }
  result->upper = r.value;

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

  //if(OK != ipv4_str_to_u32(addrs.data[0], &result->lower)) return get_error_code();

  r_u8 x = u8_from_str(addrs.data[1]);
  if(x.status != OK){
    return x.status;
  }

  result->upper = result->lower + (1 << result->bits);
  strings_free(&addrs);
  return OK;
}

error_t try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result)
{
  ERR_IF_NULL(ipv4_regex, "ipv4_regex");
  ERR_IF_NULL(str, "str");
  ERR_IF_NULL(result, "result");

  switch (format)
  {
    case COLON: return try_get_ipv4_colon_range(ipv4_regex, str, result);
    case SLASH: return try_get_ipv4_slash_range(ipv4_regex, str, result);
  }
}

Ipv4Range* ipv4_range_next(const Ipv4Range* self, Ipv4Range* next)
{
  //ERR_IF_NULL(self, "self");
  //ERR_IF_NULL(next, "next");

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
