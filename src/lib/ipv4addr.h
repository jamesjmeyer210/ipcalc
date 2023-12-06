#ifndef IPV4ADDR_H
#define IPV4ADDR_H

#include <stdint.h>
#include <stdbool.h>
#include <regex.h>
#include "error.h"
#include "result.h"

/// An enumeration of identifiable IPv4 formats
typedef enum ipv4_format {
  string,
  decimal
} Ipv4Format;

/// An IPv4 string and its format
typedef struct ipv4_str {
  Ipv4Format format;
  const char* str;
} Ipv4Str;

/// The result of creating an Ipv4Str from a string
TYPE_RESULT(Ipv4Str, Ipv4StrResult)

/// Creates an Ipv4StrResult from a string.
Ipv4StrResult ipv4_str_from_str(const char* ipv4_str);

typedef struct ipv4_range {
  uint32_t upper;
  uint32_t lower;
  uint8_t bits;
  uint32_t remainder;
} Ipv4Range;

/// Converts an IPv4 string, such as "1.1.1.1" to a 32-bit integer
r_u32 ipv4_str_to_u32(const Ipv4Str* ipv4_str);

error_t try_uint32_to_ipv4(uint32_t ipv4, char* result);

r_str decimal_to_ipv4(const Ipv4Str* ipv4);

typedef enum ipv4_range_format {
  COLON,
  SLASH,
} Ipv4RangeFormat;

error_t try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result);

Ipv4Range* ipv4_range_next(const Ipv4Range* self, Ipv4Range* next);

#endif
