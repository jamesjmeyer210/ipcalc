//
// Created by james on 9/21/23.
//

#ifndef LIB_LIST_H
#define LIB_LIST_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct list {
    void** data;
    size_t len;
    size_t cap;
} List;

#define LIST_FOR(SELF, INDEX) for(size_t INDEX = 0; INDEX < SELF->len; INDEX++)

#define LIST_FOR_REV(SELF, INDEX) for(size_t INDEX = SELF->len; i > 0; i--)

void list_free(List* self);

List* list_with_capacity(size_t cap);

void list_add(List* self, void* obj);

bool list_contains(const List* self, void* target, bool (*equals)(void* a, void* b));

void list_clear(List* self);

void list_for_each(const List* self, void (*func)(void* iter, size_t index));

int list_get_longest(const List* self,  int (*callback)(void* obj));

#endif //LIB_LIST_H