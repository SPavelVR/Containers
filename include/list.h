#ifndef __LIST_H__SP
#define __LIST_H__SP 1

#include "node.h"
#include "iterator.h"

#include <stddef.h>

typedef struct List List;
typedef struct EventsList EventsList;

struct EventsList
{
    void (*free)(List* list);

    int (*push_back)(List* list, void* data);
    int (*push_front)(List* list, void* data);
    int (*push_index)(List* lsit, void* data, size_t index);

    void* (*front)(List* list);
    void* (*back)(List* list);
    void* (*index)(List* list, size_t index);

    void* (*pop_front)(List* list);
    void* (*pop_back)(List* list);
    void* (*pop_index)(List* list, size_t index);

    int (*add) (List* list, void* data, int (*comp) (void*, void*) );
    void* (*remove) (List* list, void* data, int (*comp) (void*, void*));
    size_t (*find)(List* list, void* data, int (*comp) (void*, void*));
    size_t (*count)(List* list, void* data, int (*comp) (void*, void*));
    void (*sort)(List* list, int (*comp)(void*, void*));

    size_t (*size)(List* list);
    void (*func_on)(List* list, void (*func)(void*));
    Iterator* (*get_iterator)(List* list);
};


typedef struct List
{
    Node* container_begin;
    Node* container_end;
    size_t size;
    EventsList* events;
    int (*comp)(void*, void*);
} List;

List* init_list(int (*comp) (void*, void*));


#endif