#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <regex.h>
#include "ipv4addr.h"
#include "appstate.h"
#include "ipv4regex.h"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"convert", required_argument, 0, 'c'},
  {"format", required_argument, 0, 'f'},
  {"range", required_argument, 0, 'R'},
  {"print-range", required_argument, 0, 'P'},
  {0, 0, 0, 0}
};

int main(int argc, char** argv)
{
  AppState app_state = app_state_init();

  int c;
  int opt_index = 0;

  if (argc > 1)
  {
    while((c = getopt_long(argc, argv, "hc:f:R:P:", long_options, &opt_index)) != -1)
    {
      switch(c)
      {
        case 'h':
          printf("ipcalc\n");
          printf("By James Meyer\n\n");
          printf(" -h, --help\t\t\t\t\t\tPrint options\n");
          printf(" -c, --convert [ip address]\t\t\t\tBy default, converts an IP address in IP notation (x.x.x.x) to a decimal number. Use with (-f/--format) to specify the input type.\n");
          printf(" -R, --range [start ip address]:[end ip address]\tComputes the slash notation of a range of IP Addresses\n");
          printf(" -P, --print-range [ip address]\t\t\t\tPrints all of the IP addresses with a range.\n");
          break;
        case 'c':
          if(app_state.convert != NULL)
          {
            fprintf(stderr, "-c/--convert must only be invoked once\n");
            exit(1);
          }
          app_state.convert = optarg;
          break;
        case 'f':
          app_state.format = optarg;
          if(!validate_format(&app_state))
          {
            fprintf(stderr, "%s is not a valid format\n", app_state.format);
            exit(1);
          }
          break;
        case 'R':
          app_state.range = optarg;
          break;
        case 'P':
          app_state.print_range = optarg;
          break;
        default:
          printf("getopt returned character code %o \n", c);
      }
    }
  }

  if(app_state.convert && (app_state.format == NULL || str_eq(app_state.format, FORMAT_DECIMAL)))
  {
    printf("%u\n", ipv4_to_int(app_state.convert));
  }

  if(app_state.convert && str_eq(app_state.format, FORMAT_IPV4))
  {
    char* result = decimal_to_ipv4(app_state.convert);
    if(result == NULL)
    {
      fprintf(stderr, "Failed to convert %s\n", app_state.convert);
      exit(1);
    }
    printf("%s\n", result);
    free(result);
  }

  if(app_state.range)
  {
    regex_t regex = init_ipv4_regex();
    Ipv4Range range;
    if(!try_get_ipv4_range(&regex, app_state.range, COLON, &range))
    {
      fprintf(stderr, "Invalid IPV4 range %s\n", app_state.range);
      exit(1);
    }

    char* low = int_to_ipv4(range.lower);
    printf("%s/%d\n", low, range.bits);
    free(low);
  }

  if(app_state.print_range)
  {
    regex_t regex = init_ipv4_regex();

    Ipv4Range range;
    if(!try_get_ipv4_range(&regex, app_state.print_range, SLASH, &range))
    {
      fprintf(stderr, "Invalid IPV4 range %s\n", app_state.print_range);
      exit(1);
    }

    char lower[15];
    try_uint32_to_ipv4(range.lower, lower);
    char upper[15];
    try_uint32_to_ipv4(range.upper, upper);

    printf("Printing range: %s:%s\n", lower, upper);
    char ip[15];
    for(uint32_t i = range.lower; i < range.upper; i++)
    {
      try_uint32_to_ipv4(i, ip);
      printf("%s\n", ip);
    }
  }

  app_state_free(&app_state);
  exit(0);
}