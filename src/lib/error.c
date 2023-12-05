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
  memset(e.msg, '\0', 256);
  memcpy(&_error, &e, sizeof(Error));
  return error;
}

inline error_t error_new_msg(error_t error, uint16_t line, const char* file, const char* func, const char* format, ...)
{
  Error e;
  e.code = error;
  e.line = line;
  strncpy(e.file, file, 64);
  strncpy(e.func, func, 64);

  va_list args;
  va_start(args, format);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
  vsnprintf(e.msg, ERR_MSG_SIZE, format, args);
#pragma clang diagnostic pop
  va_end(args);

  memcpy(&_error, &e, sizeof(Error));
  return error;
}

inline Error error_new_get(error_t error, uint16_t line, const char* file, const char* func)
{
  Error e;
  e.code = error;
  e.line = line;
  strncpy(e.file, file, 64);
  strncpy(e.func, func, 64);
  memset(e.msg, '\0', 256);
  return e;
}

Error error_new_msg_get(error_t error, uint16_t line, const char* file, const char* func, const char* format, ...)
{
  Error e;
  e.code = error;
  e.line = line;
  strncpy(e.file, file, 64);
  strncpy(e.func, func, 64);

  va_list args;
  va_start(args, format);
  vsnprintf(e.msg, ERR_MSG_SIZE, format, args);
  va_end(args);
  return e;
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

void print_error(const Error* error, bool verbose)
{
  if(error == NULL)
  {
    fprintf(stderr, "Error: (NULL)");
    return;
  }
  if(verbose)
  {
    fprintf(stderr,"ERROR:\t%d\t%s.%d.%s: %s", error->code, error->file, error->line, error->func, error->msg);
    return;
  }
  fprintf(stderr,"ERROR: %s", error->msg);
}