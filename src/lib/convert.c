#include "convert.h"
#include <string.h>
#include "string.h"
#include "error.h"

#define MAX_UINT8 0xFF
#define MAX_UINT32 0xFFFFFFFF

r_u8 u8_from_str(const char* src)
{
  if(src == NULL) return r_u8_err(ERR(ERR_ARG_NULL));

  size_t len = strlen(src);
  if(len > 3) return r_u8_err(ERR(ERR_ARG_OUT_OF_RANGE));
  if(!strn_is_numeric(src, len)) return r_u8_err(ERR(ERR_ARG_INVALID));

  u64 x = 0;
  u64 m = 1;
  for(size_t i = len; i != 0; i--)
  {
    x += ((uint32_t)src[i - 1] - 48) * m;
    m *= 10;
  }

  if(x > MAX_UINT8) return r_u8_err(ERR(ERR_ARG_OUT_OF_RANGE));
  return r_u8_ok((u8)x);
}

r_u32 u32_from_str(const char* src)
{
  if(src == NULL) return r_u32_err(ERR(ERR_ARG_NULL));

  size_t len = strlen(src);
  if(len > 10) return r_u32_err(ERR(ERR_ARG_OUT_OF_RANGE));
  if(!strn_is_numeric(src, len)) return r_u32_err(ERR(ERR_ARG_OUT_OF_RANGE));

  u64 x = 0;
  u64 m = 1;
  for(size_t i = len; i != 0; i--)
  {
    x += ((uint32_t)src[i - 1] - 48) * m;
    m *= 10;
  }

  if(x > MAX_UINT32) return r_u32_err(ERR(ERR_ARG_OUT_OF_RANGE));
  return r_u32_ok((u32)x);
}