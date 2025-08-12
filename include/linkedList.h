#ifndef __LINKED_LIST_H__SP
#define __LINKED_LIST_H__SP 1

#include "node.h"
#include <stddef.h>

typedef struct LinkedList LinkedList;
typedef struct EventsLinkedList EventsLinkedList;

struct EventsLinkedList
{
    void (*free)(LinkedList* list);

    int (*push_back)(LinkedList* list, void* data);
    int (*push_front)(LinkedList* list, void* data);
    int (*push_index)(LinkedList* lsit, void* data, size_t index);

    void* (*front)(LinkedList* list);
    void* (*back)(LinkedList* list);
    void* (*index)(LinkedList* list, size_t index);

    void* (*pop_front)(LinkedList* list);
    void* (*pop_back)(LinkedList* list);
    void* (*pop_index)(LinkedList* list, size_t index);

    void* (*remove) (LinkedList* list, void* data, int (*comp) (void*, void*));

    size_t (*find)(LinkedList* list, void* data, int (*comp) (void*, void*));
    size_t (*size)(LinkedList* list);
    void (*func_on)(LinkedList* list, void (*func)(void*));
    void (*sort)(LinkedList* list, int (*comp)(void*, void*));
    Iterator* (*get_iterator)(LinkedList* list);
};


typedef struct LinkedList
{
    NodeOne* container;
    size_t size;
    EventsLinkedList* events;
    int (*comp)(void*, void*);
} LinkedList;

LinkedList* init_linked_list(int (*comp) (void*, void*));


#endif