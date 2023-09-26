#ifndef IPV4ADDR_H
#define IPV4ADDR_H

#include <stdint.h>
#include <stdbool.h>
#include <regex.h>
#include "lib/error.h"

typedef struct ipv4_range {
  uint32_t upper;
  uint32_t lower;
  uint8_t bits;
  uint32_t remainder;
} Ipv4Range;

error_t try_ipv4_to_uint32(const char* ipv4_str, uint32_t* result);

error_t try_uint32_to_ipv4(uint32_t ipv4, char* result);

error_t try_decimal_to_ipv4(const char* ipv4, char* result);

typedef enum ipv4_range_format {
  COLON,
  SLASH,
} Ipv4RangeFormat;

error_t try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result);

Ipv4Range* ipv4_range_next(const Ipv4Range* self, Ipv4Range* next);

#endif