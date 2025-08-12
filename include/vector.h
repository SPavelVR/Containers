#ifndef __VECTOR_H__SP
#define __VECTOR_H__SP 1
#include "iterator.h"
#include <stddef.h>

typedef struct Vector Vector;
typedef struct EventsVector EventsVector;

struct EventsVector
{
    void (*free)(Vector* vector);

    int (*push_back)(Vector* vector, void* data);
    int (*push_front)(Vector* vector, void* data);
    int (*push_index)(Vector* lsit, void* data, size_t index);

    void* (*front)(Vector* vector);
    void* (*back)(Vector* vector);
    void* (*index)(Vector* vector, size_t index);

    void* (*pop_front)(Vector* vector);
    void* (*pop_back)(Vector* vector);
    void* (*pop_index)(Vector* vector, size_t index);

    size_t (*find)(Vector* vector, void* data, int (*comp) (void*, void*));
    size_t (*size)(Vector* vector);
    void (*func_on)(Vector* vector, void (*func)(void*));
    void (*sort)(Vector* vector, int (*comp)(void*, void*));
    Iterator* (*get_iterator)(Vector* vector);
};


typedef struct Vector
{
    void** container;
    size_t size;
    size_t capacity;
    EventsVector* events;
    int (*comp)(void*, void*);
} Vector;

Vector* init_vector(size_t capacity, int (*comp) (void*, void*));


#endif