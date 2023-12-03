//
// Created by james on 9/21/23.
//

#ifndef LIB_STRING_H
#define LIB_STRING_H

#include "list.h"
#include "array.h"

TYPE_ARRAY(char)

typedef struct strings
{
  char** data;
  size_t count;
  size_t len;
  char* _mem;
} Strings;

typedef struct index_of_result {
  size_t index;
  bool found;
} IndexOfResult;

void strings_free(Strings* self);

char* str_new_filled(size_t len, char c);

bool str_eq(const char* a, const char* b);

IndexOfResult strn_index_of(const char* self, size_t len, char target);

void str_nreplace(char* src, size_t len, char target, char c);

bool strn_is_numeric(const char* src, size_t len);

char* str_nsubstr(const char* src, size_t start, size_t end, char* dest, size_t max_len);

array(char)* str_nsplit(const char* src, size_t len, char delim, char* dest, array(char)* result);

Strings str_split(const char* src, char delim);

char* str_trim_whitespace(char* src);

#endif //LIB_STRING_H