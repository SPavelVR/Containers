#ifndef __HESH_TABLE_H
#define __HESH_TABLE_H

#include "iterator.h"
#include "linkedList.h"
#include "hash.h"
#include <stddef.h>

typedef struct CellHashTable {
    uint64_t key;
    void* data;
} CellHashTable;

CellHashTable* init_cell_hash_table(uint64_t key, void* data);
int comp_hash_key(uint64_t key1, uint64_t key2);
void free_cell_hash_table(CellHashTable* cell);

typedef struct HashTable HashTable;
typedef struct EventsHashTable EventsHashTable;

struct EventsHashTable {
    void (*free)(HashTable* ht);

    void* (*getKey)(HashTable* ht, void* data);

    int (*push)(HashTable* ht, void* data);
    void* (*set)(HashTable* ht, void* data);
    void* (*update)(HashTable* ht, void* data);

    void* (*pop)(HashTable* ht);
    void* (*removeKey) (HashTable* ht, void* data);

    size_t (*size)(HashTable* ht);
    void (*func_on)(HashTable* ht, void (*func)(void*));
    Iterator* (*get_iterator)(HashTable* ht);
    LinkedList* (*get_list) (HashTable* ht);
};

typedef struct HashTable
{
    LinkedList** containers;
    size_t count;
    size_t capacity;
    uint64_t (*hash)(void* data);
    EventsHashTable* events;
} HashTable;

HashTable* init_hash_table(size_t capacity, uint64_t (*hash)(void*));


#endif