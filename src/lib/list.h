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

void list_free(List* self);

List* list_with_capacity(size_t cap);

void list_add(List* self, void* obj);

bool list_contains(const List* self, void* target, bool (*equals)(void* a, void* b));

int list_get_longest(const List* self,  int (*callback)(void* obj));

#endif //LIB_LIST_H