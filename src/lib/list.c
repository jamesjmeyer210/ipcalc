#include "list.h"

void list_free(List* self)
{
    free(self->data);
    free(self);
}

List* list_with_capacity(size_t cap)
{
    List* self = malloc(sizeof(List*));
    self->data = malloc(sizeof(void*) * cap);
    self->cap = cap;
    self->len = 0;
    return self;
}

static void resize(List* self)
{
    size_t new_cap = self->cap * 2;
    void** data = malloc(sizeof(void*) * new_cap);

    for(size_t i = 0; i < self->len; i++)
    {
        data[i] = self->data[i];
    }

    free(self->data);
    self->data = data;
    self->cap = new_cap;
}

void list_add(List* self, void* obj)
{
    if(self == NULL || obj == NULL) return;

    if(self->len == self->cap) resize(self);

    self->data[self->len] = obj;
    self->len++;
}

bool list_contains(const List* self, void* target, bool (*equals)(void* a, void* b))
{
  if(self == NULL) return false;
  if(target == NULL) return false;

  for(size_t i = 0; i < self->len; i++)
  {
    if(equals(target, self->data[i])) return true;
  }

  return false;
}

int list_get_longest(const List* self,  int (*callback)(void* obj))
{
    if(self == NULL) return -1;

    int longest = 0;
    for(size_t i = 0; i < self->len; i++)
    {
        int len = callback(self->data[i]);
        if(len > longest)
        {
            longest = len;
        }
    }
    return longest;
}