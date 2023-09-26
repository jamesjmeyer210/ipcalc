#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "appstate.h"
#include "lib/error.h"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"verbose", no_argument, 0, 'v'},
  {"convert", required_argument, 0, 'c'},
  {"format", required_argument, 0, 'f'},
  {"range", required_argument, 0, 'R'},
  {"print-range", required_argument, 0, 'P'},
  {0, 0, 0, 0}
};

int main(int argc, char** argv)
{
  AppState app_state = app_state_init();
  error_t status = OK;

  int c;
  int opt_index = 0;

  if (argc > 1)
  {
    while((c = getopt_long(argc, argv, "hvc:f:R:P:", long_options, &opt_index)) != -1)
    {
      switch(c)
      {
        case 'h':
          printf("ipcalc\n\n");
          printf(" -h, --help\t\t\t\t\t\tPrint options\n");
          printf(" -c, --convert [ip address]\t\t\t\tBy default, converts an IP address in IP notation (x.x.x.x) to a decimal number. Use with (-f/--format) to specify the input type.\n");
          printf(" -R, --range [start ip address]:[end ip address]\tComputes the slash notation of a range of IP Addresses\n");
          printf(" -P, --print-range [ip address]\t\t\t\tPrints all of the IP addresses with a range.\n");
          break;
        case 'v':
          app_state.verbose = true;
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

  if(app_state.convert)
  {
    status = app_state_convert(&app_state);
  }

  if(app_state.range)
  {
    status = app_state_compute_range(&app_state);
  }

  if(app_state.print_range)
  {
    status = app_state_print_range(&app_state);
  }

  app_state_free(&app_state);
  exit(status);
}