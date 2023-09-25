#ifndef IPV4ADDR_H
#define IPV4ADDR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct ipv4_range {
  uint32_t upper;
  uint32_t lower;
  uint8_t bits;
} Ipv4Range;

uint8_t ipv4_space_to_byte(const char* ipv4_space);

uint32_t ipv4_to_int(const char* ipv4_str);

char* try_uint32_to_ipv4(uint32_t ipv4, char* result);

char* int_to_ipv4(uint32_t ipv4);

char* decimal_to_ipv4(const char* ipv4);

typedef enum ipv4_range_format {
  COLON,
  SLASH,
} Ipv4RangeFormat;

bool try_get_ipv4_range(const regex_t* ipv4_regex, const char* str, Ipv4RangeFormat format, Ipv4Range* result);

#endif