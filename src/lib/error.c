//
// Created by james on 9/25/23.
//

#include <stdio.h>
#include <string.h>
#include "error.h"

static Error _error = {0};

inline error_t error_new(error_t error, uint16_t line, const char* file, const char* func)
{
  Error e;
  e.code = error;
  e.line = line;
  strncpy(e.file, file, 64);
  strncpy(e.func, func, 64);
  memcpy(&_error, &e, sizeof(Error));
  return error;
}

Error get_error()
{
  Error e = _error;
  return e;
}

error_t get_error_code()
{
  return _error.code;
}

void print_error(const Error* error)
{
  if(error == NULL)
    fprintf(stderr, "Error: (NULL)");
  else
    fprintf(stderr,"ERROR:\n\tcode: %d\n\tfile: %s\n\tline: %d\n\tfunction: %s\n", error->code, error->file, error->line, error->func);
}