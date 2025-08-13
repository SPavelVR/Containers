
#include "hashTable.h"
#include <stdlib.h>

#define __PLUS_MEMORY 16

CellHashTable* init_cell_hash_table(uint64_t key, void* data) {
    CellHashTable* cell = malloc(sizeof(CellHashTable));
    if (cell == NULL) return NULL;
    cell->key = key;
    cell->data = data;
    return cell;
};

int comp_hash_key(uint64_t key1, uint64_t key2) {
    if (key1 == key2) return 0;
    return (int) key1 - (int) key2;
};

void free_cell_hash_table(CellHashTable* cell) {
    if (cell == NULL) return    ;
    free(cell);
};

int comp_cell_hash_table(void* ptr1, void* ptr2) {
    if (ptr1 == NULL || ptr2 == NULL) return 0;
    return comp_hash_key( ((CellHashTable*)ptr1)->key, ((CellHashTable*)ptr1)->key);
}

void push_memory_hash_table(HashTable* ht, size_t capacity) {
    if (capacity == 0 || ht == NULL) return   ;
    if (ht->containers == NULL) {
        ht->containers = calloc(capacity, sizeof(LinkedList*));
        if (ht->containers == NULL) return  ;
        ht->capacity = capacity;
        for (size_t i = 0; i < capacity; i++)
        {
            ht->containers[i] = init_linked_list(comp_cell_hash_table);
        }
    }
    else {
        ht->containers = realloc(ht->containers, (ht->capacity + capacity) * sizeof(LinkedList*));
        if (ht->containers == NULL) return  ;

        for (size_t i = ht->capacity; i < ht->capacity + capacity; i++)
        {
            ht->containers[i] = init_linked_list(comp_cell_hash_table);
        }
    }
}

void update_lists_ht(HashTable* ht) {
    if (ht == NULL || ht->containers == NULL || ht->capacity == 0 || ht->containers[ht->capacity - 1]->size != 0) return    ;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        Iterator* iter = ht->containers[i]->events->get_iterator(ht->containers[i]);
        int flag = 0;
        size_t index = 0;

        for_iterator(iter) {
            CellHashTable* cell = (CellHashTable*) (iter->get_data(iter));

            if (flag) {
                flag = 0;
                CellHashTable* old_cell = (CellHashTable*) (ht->containers[i]->events->pop_index(ht->containers[i], index));
                ht->events->push(ht, old_cell->data);
                free_cell_hash_table(old_cell);
            }

            if (cell->key % ht->capacity != i) {
                index = ht->containers[i]->events->find(ht->containers[i], ((CellHashTable*) iter->iter), NULL);
                flag = 1;
            }

        }

        if (flag) {
            flag = 0;
            CellHashTable* old_cell = (CellHashTable*) (ht->containers[i]->events->pop_index(ht->containers[i], index));
            ht->events->push(ht, old_cell->data);
            free_cell_hash_table(old_cell);
        }

        free(iter);
    }
}

void check_size_hash_table(HashTable* ht) {
    if (ht == NULL || ht->count <= 200) return;

    size_t max = (ht->count * 2) / 3;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        if (ht->containers[i]->size > max) {
            push_memory_hash_table(ht, __PLUS_MEMORY);
            update_lists_ht(ht);
            return  ;
        }
    }
    
}

static EventsHashTable* events = NULL;

void free_hash_table (HashTable* ht) {
    if (ht == NULL) return;
    
    for (size_t i = 0; i < ht->capacity; i++)
    {
        ht->containers[i]->events->func_on(ht->containers[i], (void (*)(void*)) free_cell_hash_table);
        ht->containers[i]->events->free(ht->containers[i]);
        ht->containers[i] = NULL;
    }

    free(ht->containers);
    ht->containers = NULL;
    ht->capacity = 0;
    free(ht);
};

void* getKey_hash_table(HashTable* ht, void* data) {
    if (ht == NULL || ht->hash == NULL || data == NULL) return NULL;

    uint64_t key = ht->hash(data);

    LinkedList* list = ht->containers[key % ht->capacity];
    
    Iterator* iter = list->events->get_iterator(list);
    if (iter == NULL)  {
        return NULL;
    }

    for_iterator(iter) {
        if (key == ((CellHashTable*)iter->get_data(iter))->key) {
            void* data =  ((CellHashTable*)iter->get_data(iter))->data;
            free(iter);
            return data;
        }
    }
    free(iter);
    return NULL;
};

int push_hash_table(HashTable* ht, void* data) {
    if (ht == NULL || ht->capacity == 0 || data == NULL) return 0;

    if (getKey_hash_table(ht, data)) return 0;
    
    CellHashTable* cell = init_cell_hash_table(ht->hash(data), data);
    
    int r = ht->containers[cell->key % ht->capacity]->events->add(ht->containers[cell->key % ht->capacity], cell, NULL);

    if (r == 0) {
        free_cell_hash_table(cell);
        return 0;
    }
    ht->count++;
    check_size_hash_table(ht);
    return 1;
};

void* set_hash_table(HashTable* ht, void* data) {
    if (ht == NULL || ht->capacity == 0 || data == 0) return NULL;
    CellHashTable* cell = init_cell_hash_table(ht->hash(data), data);
    if (getKey_hash_table(ht, data)) {
        CellHashTable* temp = (CellHashTable*) ht->containers[cell->key % ht->capacity]->events->remove(ht->containers[cell->key % ht->capacity], cell, NULL);
        void* old_data = temp->data;
        temp->data = data;
        ht->containers[cell->key % ht->capacity]->events->add(ht->containers[cell->key % ht->capacity], temp, NULL);
        free_cell_hash_table(cell);
        return old_data;
    }
    int r = ht->containers[cell->key % ht->capacity]->events->add(ht->containers[cell->key % ht->capacity], cell, NULL);
    if (r == 0) {
        free_cell_hash_table(cell);
        return NULL;
    }
    ht->count++;
    check_size_hash_table(ht);
    return data;
};

void* update_hash_table(HashTable* ht, void* data) {
    if (ht == NULL || ht->capacity == 0 || data == 0) return NULL;
    CellHashTable* cell = init_cell_hash_table(ht->hash(data), data);
    if (getKey_hash_table(ht, data)) {
        CellHashTable* temp = (CellHashTable*) ht->containers[cell->key % ht->capacity]->events->remove(ht->containers[cell->key % ht->capacity], cell, NULL);
        void* old_data = temp->data;
        temp->data = data;
        ht->containers[cell->key % ht->capacity]->events->add(ht->containers[cell->key % ht->capacity], temp, NULL);
        free_cell_hash_table(cell);
        return old_data;
    }
    free_cell_hash_table(cell);
    return NULL;
};

void* pop_hash_table(HashTable* ht) {
    if (ht == NULL || ht->events->size(ht) == 0) return NULL;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        if (ht->containers[i]->size > 0) {
            CellHashTable* cell = ht->containers[i]->events->pop_front(ht->containers[i]);
            void* data = cell->data;
            free_cell_hash_table(cell);
            ht->count--;
            return data;
        }
    }
    return NULL;
};

size_t size_hash_table(HashTable* ht) {
    if (ht == NULL) return 0;
    return ht->count;
};

void func_on_hash_table(HashTable* ht, void (*func)(void*)) {
    if (ht == NULL || func == NULL) return  ;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        ht->containers[i]->events->func_on(ht->containers[i], func); 
    }
    
};
void* removeKey_hash_table (HashTable* ht, void* data) {
    if (ht == NULL || ht->count == 0 || data == NULL) return NULL;

    CellHashTable cell =  {ht->hash(data), data};
    
    LinkedList* list = ht->containers[cell.key % ht->capacity];

    CellHashTable* res = (CellHashTable*) list->events->remove(list, &cell, NULL);
    
    if (res == NULL) return NULL;
    ht->count--;
    void* old_data = res->data;
    free_cell_hash_table(res);
    return old_data;
};

LinkedList* get_list_from_hash_table(HashTable* ht) {
    if (ht == NULL || ht->count == 0) return NULL;

    LinkedList* list = init_linked_list(comp_cell_hash_table);
    if (list == NULL) return NULL;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        Iterator* iter = ht->containers[i]->events->get_iterator(ht->containers[i]);

        for_iterator(iter) {
            void* data = iter->get_data(iter);
            list->events->push_front(list, data);
        }

        free(iter);
    }

    return list;
};

Iterator* get_iterator_hash_table(HashTable* ht) {
    if (ht == NULL || ht->count == 0) return NULL;
    LinkedList* list = get_list_from_hash_table(ht);
    if (list == NULL) return NULL;
    Iterator* iter = list->events->get_iterator(list);
    free(list);
    return iter;
};

HashTable* init_hash_table(size_t capacity, uint64_t (*hash)(void*)) {
    HashTable* ht = malloc(sizeof(HashTable));

    if (ht == NULL) return NULL;

    ht->containers = NULL;
    ht->count = 0;
    ht->capacity = 0;
    ht->hash = hash;

    if (capacity == 0) capacity = __PLUS_MEMORY;
    push_memory_hash_table(ht, capacity);

    if (events == NULL) {
        events = malloc(sizeof(*events));
        if (events != NULL) {
            events->free = free_hash_table;

            events->push = push_hash_table;
            events->set = set_hash_table;
            events->update = update_hash_table;

            events->getKey = getKey_hash_table;
            events->pop = pop_hash_table;
            events->removeKey = removeKey_hash_table;

            events->size = size_hash_table;
            events->func_on = func_on_hash_table;
            events->get_iterator = get_iterator_hash_table;
            events->get_list = get_list_from_hash_table;
        }
    }

    ht->events = events;
    return ht;
};