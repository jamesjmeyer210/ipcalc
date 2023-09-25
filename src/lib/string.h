//
// Created by james on 9/21/23.
//

#ifndef LIB_STRING_H
#define LIB_STRING_H

#include "list.h"

bool str_eq(const char* a, const char* b);

bool strn_is_numeric(const char* src, size_t len);

List* str_split(const char* src, char delim);

char* str_new_filled(int len, char c);

char* str_trim_whitespace(char* src);

#endif //LIB_STRING_H