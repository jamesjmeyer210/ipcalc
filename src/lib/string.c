#include <string.h>
#include <stdbool.h>
#include "stdlib.h"
#include "string.h"

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

static int str_count_char(const char* src, size_t len, char c)
{
    int count = 0;
    for(size_t i = 0; i < len; i++)
    {
        if(src[i] == c)
        {
            count++;
        }
    }
    return count;
}

static int str_index_of_char_in_range(const char* src, char c, int start, int end)
{
    for(int i = start; i < end; i++)
    {
        if(src[i] == '\0') return -1;
        if(src[i] == c) return i;
    }
    return -1;
}

static char* str_substring(const char* src, int start, int end)
{
    int size = end - start + 1;
    char* sub = malloc(size);
    memcpy(sub, &src[start], size);
    sub[size - 1] = '\0';
    return sub;
}

List* str_split(const char* src, char delim)
{
    if(src == NULL) return NULL;
    if(delim == '\0') return NULL;

    size_t len = strlen(src);

    int count = str_count_char(src, len, delim);
    if(count == 0) return NULL;

    int size = count + 1;
    List* list = list_with_capacity(size);

    int start = 0;
    for(int i = 0; i < size; i++)
    {
        int end = str_index_of_char_in_range(src, delim, start, (int)len);
        if(end == -1)
        {
            list_add(list, str_substring(src, start, (int)len));
        }
        else
        {
            list_add(list, str_substring(src, start, end));
        }
        start = end + 1;
    }

    return list;
}

char* str_new_filled(int len, char c)
{
    char* str = malloc(len);
    for(int i = 0; i < len; i++)
    {
        str[i] = c;
    }
    str[len] = '\0';
    return str;
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