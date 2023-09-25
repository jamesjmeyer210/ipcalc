#include "ipv4regex.h"
#include "ipv4addr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/error.h"
#include "lib/string.h"
#include "lib/convert.h"
#include "lib/bits.h"

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

error_t try_ipv4_to_int32(const char* ipv4_str, uint32_t* result)
{
  regex_t regex = init_ipv4_regex();
  if(!regex_is_valid(&regex, ipv4_str))
  {
    return ERR(ERR_ARG_INVALID);
    //fprintf(stderr, "%s is not a valid IPv4 address\n", ipv4_str);
    //exit(1);
  }

  List* spaces = str_split(ipv4_str, '.');
  if(spaces->len != 4)
  {
    list_free(spaces);
    return ERR(ERR_ARG_INVALID);
  }

  uint8_t x1 = ipv4_space_to_byte(spaces->data[0]);
  uint8_t x2 = ipv4_space_to_byte(spaces->data[1]);
  uint8_t x3 = ipv4_space_to_byte(spaces->data[2]);
  uint8_t x4 = ipv4_space_to_byte(spaces->data[3]);

  list_free(spaces);
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
  List* addrs = str_split(str, ':');
  if(addrs->len != 2)
  {
    list_free(addrs);
    return ERR(ERR_ARG_INVALID);
  }

  for(size_t i = 0; i < addrs->len; i++)
  {
    if(!regex_is_valid(ipv4_regex, addrs->data[i]))
    {
      list_free(addrs);
      return ERR(ERR_ARG_INVALID);
    }
  }

  if(OK != try_ipv4_to_int32(addrs->data[0], &result->upper)) return get_error_code();
  if(OK != try_ipv4_to_int32(addrs->data[1], &result->upper)) return get_error_code();

  result->bits = count_bits(result->upper - result->lower);

  list_free(addrs);
  return OK;
}

static error_t try_get_ipv4_slash_range(const regex_t* regex, const char* str, Ipv4Range* result)
{
  List* addrs = str_split(str, '/');
  if(addrs->len != 2)
  {
    list_free(addrs);
    return ERR(ERR_ARG_INVALID);
  }

  if(!regex_is_valid(regex, addrs->data[0]))
  {
    list_free(addrs);
    return ERR(ERR_ARG_INVALID);
  }

  if(OK != try_ipv4_to_int32(addrs->data[0], &result->lower)) return get_error_code();
  if(OK != try_uint8_from_str(addrs->data[1], &result->bits)) return get_error_code();
  result->upper = result->lower + (1 << result->bits);
  list_free(addrs);
  return true;
}

error_t try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result)
{
  if(str == NULL) return false;

  switch (format)
  {
    case COLON: return try_get_ipv4_colon_range(ipv4_regex, str, result);
    case SLASH: return try_get_ipv4_slash_range(ipv4_regex, str, result);
  }
}