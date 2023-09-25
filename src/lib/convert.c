#include "convert.h"
#include <string.h>
#include "string.h"

#define MAX_UINT8 0xFF
#define MAX_UINT32 0xFFFFFFFF

uint8_t uint8_from_str(const char* src)
{
  if(src == NULL) return 0;

  size_t len = strlen(src);
  if(len > 3) return 0;
  if(!strn_is_numeric(src, len)) return 0;

  uint64_t x = 0;
  uint64_t m = 1;
  for(size_t i = len; i != 0; i--)
  {
    x += ((uint32_t)src[i - 1] - 48) * m;
    m *= 10;
  }

  if(x > MAX_UINT8) return 0;
  return (uint32_t)x;
}

uint32_t uint32_from_str(const char* src)
{
  if(src == NULL) return 0;

  size_t len = strlen(src);
  if(len > 10) return 0;
  if(!strn_is_numeric(src, len)) return 0;

  uint64_t x = 0;
  uint64_t m = 1;
  for(size_t i = len; i != 0; i--)
  {
    x += ((uint32_t)src[i - 1] - 48) * m;
    m *= 10;
  }

  if(x > MAX_UINT32) return 0;
  return (uint32_t)x;
}