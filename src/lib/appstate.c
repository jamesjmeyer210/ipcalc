#include "appstate.h"
#include "ipv4addr.h"
#include "ipv4regex.h"
#include "file.h"
#include <stdio.h>

AppState app_state_init()
{
  AppState state = {0};
  state._formats = array_with_cap(char)(2);
  state._formats.data[0] = FORMAT_IPV4;
  state._formats.data[1] = FORMAT_DECIMAL;

  return state;
}

void app_state_free(AppState* self)
{
  array_free(char)(&self->_formats);
}

#define LOG_DEBUG(self, format, ...) if(self->verbose) printf(format, __VA_ARGS__)

inline bool validate_format(const AppState* self)
{
  return array_contains(char)(&self->_formats, self->format, (bool (*)(const char *, const char *)) str_eq);
}

inline error_t app_state_convert(const AppState* self)
{
  if(self->format == NULL || str_eq(self->format, FORMAT_DECIMAL))
  {
    LOG_DEBUG(self, "DBG: Attempting to convert %s format\n", FORMAT_DECIMAL);

    uint32_t x;
    if(OK != try_ipv4_to_uint32(self->convert, &x))
    {
      Error e = get_error();
      print_error(&e, self->verbose);
      return e.code;
    }
    printf("%u\n", x);
  }

  if(str_eq(self->format, FORMAT_IPV4))
  {
    LOG_DEBUG(self, "DBG: Attempting to convert %s format\n", FORMAT_IPV4);

    char result[15];
    if(OK != try_decimal_to_ipv4(self->convert, result))
    {
      Error e = get_error();
      print_error(&e, self->verbose);
      return e.code;
    }
    printf("%s\n", result);
  }

  return OK;
}

inline error_t app_state_compute_range(const AppState* self)
{
  regex_t regex = init_ipv4_regex();
  Ipv4Range range = {0};
  if(OK != try_get_ipv4_range(&regex, self->range, COLON, &range))
  {
    Error e = get_error();
    print_error(&e, self->verbose);
    return e.code;
  }

  char low[15];
  if(OK != try_uint32_to_ipv4(range.lower, low))
  {
    Error e = get_error();
    print_error(&e, self->verbose);
    return e.code;
  }

  printf("%s/%d\n", low, range.bits);
  while(range.remainder > 0)
  {
    Ipv4Range next = {0};
    next = *ipv4_range_next(&range, &next);
    try_uint32_to_ipv4(next.lower, low);
    if(next.remainder > 0)
    {
      printf("%s/%d\n", low, next.bits);
    }
    else
    {
      printf("%s\n", low);
    }
    range = next;
  }
  return OK;
}

inline error_t app_state_print_range(const AppState* self)
{
  regex_t regex = init_ipv4_regex();

  Ipv4Range range;
  if(OK != try_get_ipv4_range(&regex, self->print_range, SLASH, &range))
  {
    Error e = get_error();
    print_error(&e, self->verbose);
    return e.code;
  }

  char lower[15];
  char upper[15];
  try_uint32_to_ipv4(range.lower, lower);
  try_uint32_to_ipv4(range.upper, upper);

  printf("Printing range: %s:%s\n", lower, upper);
  char ip[15];
  for(uint32_t i = range.lower; i < range.upper; i++)
  {
    try_uint32_to_ipv4(i, ip);
    printf("%s\n", ip);
  }

  return OK;
}

inline error_t app_state_print_groups(const AppState* self)
{
  List* lines = NULL;
  if(OK != file_read_all_lines("countries.csv", lines))
  {
    Error e = get_error();
    print_error(&e, self->verbose);
    return e.code;
  }

  LIST_FOR(lines, i)
  {
    printf("[%zu] %s\n", i, (char*)lines->data[i]);
  }

  list_free(lines);
  return OK;
}