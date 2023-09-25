#include "appstate.h"

AppState app_state_init()
{
  AppState state = {0};
  state._formats = list_with_capacity(2);
  list_add(state._formats, FORMAT_DECIMAL);
  list_add(state._formats, FORMAT_IPV4);

  return state;
}

void app_state_free(AppState* self)
{
  list_free(self->_formats);
}

inline bool validate_format(const AppState* self)
{
  return list_contains(self->_formats, self->format, (bool (*)(void *, void *)) str_eq);
}