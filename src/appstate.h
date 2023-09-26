//
// Created by james on 9/21/23.
//

#ifndef APPSTATE_H
#define APPSTATE_H

#include "stdbool.h"
#include "lib/error.h"
#include "lib/string.h"
#include "lib/list.h"

#define FORMAT_DECIMAL "decimal"
#define FORMAT_IPV4 "ipv4"

typedef struct app_state
{
  bool help;
  bool verbose;
  char* convert;
  char* range;
  char* print_range;
  char* format;
  List* _formats;
} AppState;

AppState app_state_init();

void app_state_free(AppState* self);

bool validate_format(const AppState* self);

error_t app_state_convert(const AppState* self);

error_t app_state_compute_range(const AppState* self);

error_t app_state_print_range(const AppState* self);

#endif //APPSTATE_H