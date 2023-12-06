#include <assert.h>
#include "types.h"
#include "error.h"

#ifndef IPCALC_RESULT_H
#define IPCALC_RESULT_H

#define TYPE_RESULT(T, T_NAME) \
typedef struct T_NAME          \
{                              \
  error_t status;              \
  T value;                     \
} T_NAME;                      \
\
T_NAME T_NAME##_err(error_t e);\
\
T_NAME T_NAME##_ok(T value);

#define IMPL_RESULT(T, T_NAME) \
                               \
inline T_NAME T_NAME##_err(error_t e) \
{                              \
  T_NAME self = { .status = e }; \
  return self;                 \
}\
\
inline T_NAME T_NAME##_ok(T value)    \
{                              \
  T_NAME self = { .status = OK, .value = value }; \
  return self;                 \
}

TYPE_RESULT(u8, r_u8)
TYPE_RESULT(u32, r_u32)
TYPE_RESULT(char*, r_str)
TYPE_RESULT(const char*, r_const_str)

#endif // IPCALC_RESULT_H