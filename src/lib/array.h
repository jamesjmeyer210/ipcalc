#ifndef IPCALC_ARRAY_H
#define IPCALC_ARRAY_H

#include <assert.h>
#include <stddef.h>

#define array(T) struct array_##T

#define array_init(T) array_##T##_init

#define array_free(T) array_##T##_free

#define array_with_cap(T) array_##T##_with_cap

#define array_contains(T) array_##T##_contains

#define TYPE_ARRAY(T) \
array(T)              \
{                     \
  T** data;           \
  size_t len;         \
};                    \
                      \
array(T) array_init(T)(T** src, size_t len); \
                      \
array(T) array_with_cap(T)(size_t cap); \
                      \
void array_free(T)(array(T)* self); \
                      \
bool array_contains(T)(const array(T)* self, T* target, bool (*equals)(const T* a, const T* b));

#define IMPL_ARRAY(T) \
array(T) array_init(T)(T** src, size_t len) \
{                     \
  array(T) self;      \
  self.data = src;    \
  self.len = len;     \
  return self;        \
}                     \
                      \
void array_free(T)(array(T)* self) \
{                     \
  free(self->data);   \
  self->len = 0;      \
}                     \
                      \
array(T) array_with_cap(T)(size_t cap)      \
{                     \
  T** mem = malloc(sizeof(T*) * cap);       \
  return array_init(T)(mem, cap);           \
}                     \
                      \
bool array_contains(T)(const array(T)* self, T* target, bool (*equals)(const T* a, const T* b)) \
{                     \
  assert(self != NULL);                     \
  if(target == NULL)  \
    return false;     \
                      \
  for(size_t i = 0; i < self->len; i++)     \
  { \
    if(equals(target, self->data[i]))       \
      return true; \
  } \
  \
  return false; \
}                     \


#endif //IPCALC_ARRAY_H