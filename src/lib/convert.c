#include "convert.h"
#include <string.h>
#include "string.h"
#include "error.h"

#define MAX_UINT8 0xFF
#define MAX_UINT32 0xFFFFFFFF

error_t try_uint8_from_str(const char* src, uint8_t* result)
{
  if(src == NULL) return ERR(ERR_ARG_NULL);
  if(result == NULL) return ERR(ERR_ARG_NULL);

  size_t len = strlen(src);
  if(len > 3) return ERR(ERR_ARG_OUT_OF_RANGE);
  if(!strn_is_numeric(src, len)) return ERR(ERR_ARG_INVALID);

  uint64_t x = 0;
  uint64_t m = 1;
  for(size_t i = len; i != 0; i--)
  {
    x += ((uint32_t)src[i - 1] - 48) * m;
    m *= 10;
  }

  if(x > MAX_UINT8) return ERR(ERR_ARG_OUT_OF_RANGE);
  *result = (uint8_t)x;
  return OK;
}

error_t try_uint32_from_str(const char* src, uint32_t* result)
{
  if(src == NULL) return ERR(ERR_ARG_NULL);
  if(result == NULL) return ERR(ERR_ARG_NULL);

  size_t len = strlen(src);
  if(len > 10) return ERR(ERR_ARG_OUT_OF_RANGE);
  if(!strn_is_numeric(src, len)) return ERR(ERR_ARG_OUT_OF_RANGE);

  uint64_t x = 0;
  uint64_t m = 1;
  for(size_t i = len; i != 0; i--)
  {
    x += ((uint32_t)src[i - 1] - 48) * m;
    m *= 10;
  }

  if(x > MAX_UINT32) return ERR(ERR_ARG_OUT_OF_RANGE);
  *result = (uint32_t)x;
  return OK;
}