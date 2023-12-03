#include <assert.h>
#include "list.h"

void list_free(List* self)
{
  free(self->data);
  free(self);
}

List list_with_capacity(size_t type_size, size_t cap)
{
    List self = {0};
    self.type_size = type_size;
    self.data = malloc(sizeof(void*) * cap);
    self.cap = cap;
    self.len = 0;
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

bool list_contains(const List* self, void* target, bool (*equals)(const void* a, const void* b))
{
  if(self == NULL) return false;
  if(target == NULL) return false;

  for(size_t i = 0; i < self->len; i++)
  {
    const void* item = (char*)self->data + (i * self->type_size);
    if(equals(target, item)) return true;
  }

  return false;
}

void list_for_each(const List* self, void (*func)(const void* iter, size_t index))
{
  assert(self != NULL);
  assert(func != NULL);

  for(size_t i = 0; i < self->len; i++)
  {
    const void* item = (char*)self->data + (i * self->type_size);
    func(item, i);
  }
}

void list_clear(List* self)
{
  self->len = 0;
}

int list_get_longest(const List* self, int (*callback)(void* obj))
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