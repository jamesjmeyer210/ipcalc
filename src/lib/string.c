#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "stdlib.h"
#include "string.h"

IMPL_ARRAY(char)

void strings_free(Strings* self)
{
  assert(self != NULL);
  free(self->data);
  free(self->_mem);
  self->len = 0;
}

char* str_new_filled(size_t len, char c)
{
  char* str = malloc(len);
  for(size_t i = 0; i < len; i++)
  {
    str[i] = c;
  }
  str[len] = '\0';
  return str;
}

inline bool str_eq(const char* a, const char* b)
{
  if(a == NULL && b == NULL) return true;
  if(a == NULL) return false;
  if(b == NULL) return false;
  return strcmp(a, b) == 0;
}

inline bool strn_is_numeric(const char* src, size_t len)
{
  if(src == NULL) return false;

  for(size_t i = 0; i < len; i++)
  {
    if((src[i] - 48) >= 10) return false;
  }
  return true;
}

static size_t str_count_char(const char* src, size_t len, char c)
{
    size_t count = 0;
    for(size_t i = 0; i < len; i++)
    {
        if(src[i] == c)
        {
            count++;
        }
    }
    return count;
}

char* str_nsubstr(const char* src, size_t start, size_t end, char* dest, size_t max_len)
{
  size_t size = end - start;
  if(size > max_len)
  {
    strncpy(dest, &src[start], max_len);
  }
  else
  {
    strncpy(dest, &src[start], size);
  }
  return dest;
}

array(char)* str_nsplit(const char* src, size_t len, char delim, char* dest, array(char)* result)
{
  assert(src != NULL);
  assert(dest != NULL);
  assert(result != NULL);
  if(delim == '\0') return NULL;

  strncpy(dest, src, len);
  strtok_r(dest, &delim, result->data);
  result->len = str_count_char(dest, len, delim);

  return result;
}

Strings str_split(const char* src, char delim)
{
  assert(src != NULL);

  size_t len = strlen(src);
  Strings strings;
  strings.data = malloc(sizeof(char*) * len);
  strings._mem = str_new_filled(len, '\0');

  array(char) x = array_init(char)(strings.data, 0);
  str_nsplit(src, len, delim, strings._mem, &x);

  strings.len = x.len;
  return strings;
}

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

char* str_trim_whitespace(char* src)
{
    if(src == NULL)
    {
        return NULL;
    }

    size_t len = strlen(src);
    if(len == 0)
    {
        return src;
    }

    size_t start = 0;
    for(char iter = src[start]; start < len && is_whitespace(iter); iter = src[start++]){}
    if(start == len)
    {
        src[0] = '\0';
        return src;
    }
    if(start > 0)
    {
        for(size_t i = 0; i < len + 1; i++)
        {
            src[i] = src[i + start - 1];
        }
        len = len - start;
    }

    size_t end = len;
    for(char iter = src[end]; end > 0 && is_whitespace(iter); iter = src[end--]){}
    src[end] = '\0';
    return src;
}