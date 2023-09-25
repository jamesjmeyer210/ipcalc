#include "ipv4regex.h"
#include "ipv4addr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/string.h"
#include "lib/convert.h"
#include "lib/bits.h"

static uint8_t from_char(const char c)
{
  uint8_t x = (uint8_t)c;
  return x - 48;
}

uint8_t ipv4_space_to_byte(const char* ipv4_space)
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

uint32_t ipv4_to_int(const char* ipv4_str)
{
  regex_t regex = init_ipv4_regex();
  if(!regex_is_valid(&regex, ipv4_str))
  {
    fprintf(stderr, "%s is not a valid IPv4 address\n", ipv4_str);
    exit(1);
  }

  List* spaces = str_split(ipv4_str, '.');

  uint8_t x1 = ipv4_space_to_byte(spaces->data[0]);
  uint8_t x2 = ipv4_space_to_byte(spaces->data[1]);
  uint8_t x3 = ipv4_space_to_byte(spaces->data[2]);
  uint8_t x4 = ipv4_space_to_byte(spaces->data[3]);

  list_free(spaces);
  return ((uint32_t)x1 << 24) + ((uint32_t)x2 << 16) + ((uint32_t)x3 << 8) + (uint32_t)x4;
}

char* try_uint32_to_ipv4(uint32_t ipv4, char* result)
{
  uint8_t x1 = ipv4 >> 24;
  uint8_t x2 = (ipv4 << 8) >> 24;
  uint8_t x3 = (ipv4 << 16) >> 24;
  uint8_t x4 = (ipv4 << 24) >> 24;

  snprintf(result, 15, "%d.%d.%d.%d", x1, x2, x3, x4);
  return result;
}

char* int_to_ipv4(uint32_t ipv4)
{
  uint8_t x1 = ipv4 >> 24;
  uint8_t x2 = (ipv4 << 8) >> 24;
  uint8_t x3 = (ipv4 << 16) >> 24;
  uint8_t x4 = (ipv4 << 24) >> 24;

  char* result = malloc(15);
  sprintf(result, "%d.%d.%d.%d", x1, x2, x3, x4);
  return result;
}

char* decimal_to_ipv4(const char* ipv4)
{
  if(ipv4 == NULL) return NULL;

  uint32_t x = uint32_from_str(ipv4);
  if(x == 0 && !str_eq("0", ipv4))
  {
    return NULL;
  }

  return int_to_ipv4(x);
}

static bool try_get_ipv4_colon_range(const regex_t* ipv4_regex, const char* str, Ipv4Range* result)
{
  List* addrs = str_split(str, ':');
  if(addrs->len != 2)
  {
    return false;
  }

  for(size_t i = 0; i < addrs->len; i++)
  {
    if(!regex_is_valid(ipv4_regex, addrs->data[i]))
    {
      return false;
    }
  }

  result->lower = ipv4_to_int(addrs->data[0]);
  result->upper = ipv4_to_int(addrs->data[1]);
  result->bits = count_bits(result->upper - result->lower);

  list_free(addrs);
  return true;
}

static bool try_get_ipv4_slash_range(const regex_t* regex, const char* str, Ipv4Range* result)
{
  List* addrs = str_split(str, '/');
  if(addrs->len != 2)
  {
    return false;
  }

  if(!regex_is_valid(regex, addrs->data[0]))
  {
    return false;
  }

  result->lower = ipv4_to_int(addrs->data[0]);
  result->bits = uint8_from_str(addrs->data[1]);
  result->upper = result->lower + (1 << result->bits);
  list_free(addrs);
  return true;
}

bool try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result)
{
  if(str == NULL) return false;

  switch (format)
  {
    case COLON: return try_get_ipv4_colon_range(ipv4_regex, str, result);
    case SLASH: return try_get_ipv4_slash_range(ipv4_regex, str, result);
  }
}