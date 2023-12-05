#include <assert.h>
#include "error.h"

#ifndef IPCALC_RESULT_H
#define IPCALC_RESULT_H

#define error_or(T) union error_or_##T

#define TYPE_ERROR_OR(T) \
error_or(T)               \
{                        \
  Error error;           \
  T result;              \
};

#define result(T) struct result_##T

#define result_ok(T) result_##T##_ok

#define result_err(T) result_##T##_err

#define result_get(T) result_##T##_get

#define result_get_err(T) result_##T##_get_err

#define TYPE_RESULT(T) \
                       \
TYPE_ERROR_OR(T)       \
                       \
result(T)              \
{                      \
  error_or(T) _inner;  \
  bool ok;             \
};                     \
                       \
result(T) result_err(T)(Error err)  \
{                      \
  result(T) self;      \
  self.ok = false;      \
  self._inner.error = err;          \
  return self;         \
}                       \
                       \
result(T) result_ok(T)(T ok)  \
{                      \
  result(T) self;      \
  self.ok = true;      \
  self._inner.result = ok;     \
  return self;         \
}                      \
                       \
T result_get(T)(result(T) *self) \
{                      \
  assert(self != NULL);\
  assert(self->ok);    \
  return self->_inner.result;     \
}                      \
                       \
Error result_get_err(T)(result(T) *self) \
{                      \
  assert(self != NULL);\
  assert(!self->ok);   \
  return self->_inner.error;     \
}

#endif // IPCALC_RESULT_H