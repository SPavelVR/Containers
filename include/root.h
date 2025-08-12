#ifndef __ROOT_H
#define __ROOT_H
#include <stddef.h>
#include "iterator.h"

typedef struct Root {
    void* data;
    struct Root *right, *left, *parent;
    size_t size;
} Root;

Root* gen_root(Root* parent);
void free_root(Root* root);

int push_root(Root* root, void* data, int (*comp)(void* , void* ));

void* get_root(Root* root);
void* get_max_root(Root* root);
void* get_min_root(Root* root);

void* pop_root(Root* root);
void* pop_max_root(Root* root);
void* pop_min_root(Root* root);

int find_root(Root* root, void* data, int (*comp)(void* , void* ));
void func_on_root(Root* root, void (*func)(void*));
Iterator* get_iterator_root(Root* root);
size_t size_root(Root* root);

#endif