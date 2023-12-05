#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "ipv4regex.h"

regex_t init_ipv4_regex()
{
  regex_t regex;
  if(regcomp(&regex, "([1-2]?[0-9]{1,2})([.][1-2]?[0-9]{1,2}){3}", REG_EXTENDED))
  {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }

  return regex;
}

regex_t init_ipv4_range_regex()
{
  regex_t regex;
  if(regcomp(&regex, "([1-2]?[0-9]{1,2})([.][1-2]?[0-9]{1,2}){3}([/]([1-9]{1}|[1-2]{1}[0-9]{1}|3[0-2]{1})){1}$", REG_EXTENDED))
  {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }

  return regex;
}

bool regex_is_valid(const regex_t* regex, const char* str)
{
  int result = regexec(regex, str, 0, NULL, 0);
  return REG_NOMATCH != result;
}