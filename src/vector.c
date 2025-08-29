
#include "..\include\vector.h"
#include <stdlib.h>

#define __PLUS_MEMERY_VECTRO 16

static EventsVector* events = NULL;

void push_memory_vactor(Vector* vector, size_t capacity) {
    if (vector == NULL || capacity == 0) return ;
    if (vector->container == NULL) {
        vector->container = calloc(capacity, sizeof(void*));
        vector->capacity = capacity;
    } else {
        vector->container = realloc(vector->container, (vector->capacity + capacity) * sizeof(void*));
        vector->capacity += capacity;
    }
}

void free_vector (Vector* vector) {
    if (vector == NULL) return ;
    if (vector->container != NULL) free(vector->container);
    free(vector);
};

int push_back_vector(Vector* vector, void* data) {
    if (vector == NULL || data == NULL) return 0;
    if (vector->container == NULL || vector->size == vector->capacity) {
        push_memory_vactor(vector, __PLUS_MEMERY_VECTRO);
    }

    vector->container[vector->size] = data;
    vector->size++;
    return 1;
};

int push_front_vector(Vector* vector, void* data) {
    if (vector == NULL || data == NULL) return 0;
    if (vector->container == NULL || vector->size == vector->capacity) {
        push_memory_vactor(vector, __PLUS_MEMERY_VECTRO);
    }

    for (size_t i = 0; i < vector->size; i++)
    {
        vector->container[vector->size - i] = vector->container[vector->size - i - 1];
    }
    
    vector->container[0] = data;
    vector->size++;
    return 1;
};

int push_index_vector(Vector* vector, void* data, size_t index) {
    if (vector == NULL || data == NULL || vector->size < index) return 0;
    if (vector->container == NULL || vector->size == vector->capacity) {
        push_memory_vactor(vector, __PLUS_MEMERY_VECTRO);
    }

    for (size_t i = index; i < vector->size; i++)
    {
        vector->container[vector->size - i] = vector->container[vector->size - i - 1];
    }
    
    vector->container[index] = data;
    vector->size++;
    return 1;
};

void* front_vector(Vector* vector) {
    if (vector == NULL || vector->container == NULL || vector->size == 0) return NULL;
    return vector->container[0];
};

void* back_vector(Vector* vector) {
    if (vector == NULL || vector->container == NULL || vector->size == 0) return NULL;
    return vector->container[vector->size - 1];
};

void* index_vector(Vector* vector, size_t index) {
    if (vector == NULL || vector->container == NULL || vector->size <= index) return NULL;
    return vector->container[index];
};

void* pop_front_vector(Vector* vector) {
    if (vector == NULL || vector->size == 0) return NULL;

    void* data = vector->container[0];

    for (size_t i = 1; i < vector->size; i++)
    {
        vector->container[i-1] = vector->container[i];
    }
    vector->container[vector->size - 1] = NULL;
    vector->size--;
    return data;
};

void* pop_back_vector(Vector* vector) {
    if (vector == NULL || vector->size == 0) return NULL;
    void* data = vector->container[vector->size - 1];
    vector->container[vector->size - 1] = NULL;
    vector->size--;
    return data;
};

void* pop_index_vector(Vector* vector, size_t index) {
    if (vector == NULL || vector->size <= index) return NULL;

    void* data = index_vector(vector, index);

    for (size_t i = index + 1; i < vector->size; i++)
    {
        vector->container[i-1] = vector->container[i];
    }

    vector->container[vector->size - 1] = NULL;
    vector->size--;
    return data;
};

void* find_vector(Vector* vector, void* data, int (*comp) (void*, void*)) {
    if (vector == NULL || vector->container == NULL || data == NULL) return NULL;

    int (*rcomp)(void*, void*) = comp;
    if (rcomp == NULL) rcomp = vector->comp;

    for (size_t i = 0; i < vector->size && rcomp != NULL; i++)
    {
        if (rcomp(vector->container[i], data)) return vector->container[i];
    }

    return NULL;
};

size_t findi_vector(Vector* vector, void* data, int (*comp) (void*, void*)) {
    if (vector == NULL || vector->container == NULL || data == NULL) return 0;

    int (*rcomp)(void*, void*) = comp;
    if (rcomp == NULL) rcomp = vector->comp;

    for (size_t i = 0; i < vector->size && rcomp != NULL; i++)
    {
        if (rcomp(vector->container[i], data)) return i;
    }
    return vector->size;
};

size_t size_vector(Vector* vector) {
    if (vector == NULL || vector->container == NULL) return 0;
    return vector->size;
};

void func_on_vector(Vector* vector, void (*func)(void*)) {
    if (vector == NULL || vector->container == NULL || func == NULL) return ;
    
    for (size_t i = 0; i < vector->size; i++) {
        func(vector->container[i]);
    }
};

void sort_vector(Vector* vector, int (*comp)(void*, void*)) {
    if (vector == NULL || vector->container == NULL) return ;

    int (*rcomp)(void*, void*) = comp;
    if (rcomp == NULL) rcomp = vector->comp;

    int flag = 0;
    size_t i = 1;

    while ( i < vector->size && rcomp != NULL) {
        if (rcomp(vector->container[i-1], vector->container[i]) > 0) {
            void* temp = vector->container[i-1];
            vector->container[i-1] = vector->container[i];
            vector->container[i] = temp;
            flag = 1;
        }
        i++;
        if (i == vector->size && flag) {
            i = 1;
            flag = 0;
        }
    }
};

void pass_iterator_vector(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return  ;
    ((void**) iter->iter)++;
};

void* get_data_iterator_vector(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return NULL;
    return *(void**) iter->iter;
};

Iterator* get_iterator_vector(Vector* vector) {
    if (vector == NULL || vector->container == NULL || vector->size == 0) return NULL;

    Iterator* iter = init_iterator(vector->container, vector->container + vector->size, pass_iterator_vector, get_data_iterator_vector);
    return iter;
};

int add_vector (Vector* vector, void* data, int (*comp) (void*, void*)) {
    if (vector == NULL || vector->container == NULL || data == NULL) return 0;

    size_t ind = find_vector(vector, data, comp);

    if (ind >= size_vector(vector)) return push_back_vector(vector, data);

    return 0;
};

void* remove_vector (Vector* vector, void* data, int (*comp) (void*, void*)) {
    if (vector == NULL || vector->container == NULL || data == NULL) return NULL;

    size_t ind = find_vector(vector, data, comp);

    if (ind < size_vector(vector)) return pop_index_vector(vector, data, ind);

    return NULL;
};

size_t count_vector (Vector* vector, void* data, int (*comp) (void*, void*)) {
    if (vector == NULL || vector->container == NULL || data == NULL) return 0;

    size_t count = 0;

    int (*rcomp)(void*, void*) = comp;
    if (rcomp == NULL) rcomp = vector->comp;

    for (size_t i = 0; i < vector->size && rcomp != NULL; i++)
    {
        if (rcomp(vector->container[i], data)) count++;
    }

    return count;
};

Vector* init_vector(size_t capacity, int (*comp) (void*, void*)) {
    Vector* vector = malloc(sizeof(Vector));
    if (vector == NULL) return NULL;
    vector->size = 0;
    vector->capacity = 0;
    vector->container = NULL;
    vector->comp = comp;
    if (events == NULL) {
        events = malloc(sizeof(EventsVector));
        if (events != NULL) {
            events->free = free_vector;

            events->back = back_vector;
            events->front = front_vector;
            events->index = index_vector;

            events->push_back = push_back_vector;
            events->push_front = push_front_vector;
            events->push_index = push_index_vector;

            events->pop_back = pop_back_vector;
            events->pop_front = pop_front_vector;
            events->pop_index = pop_index_vector;

            events->add = add_vector;
            events->remove = remove_vector;
            events->find = find_vector;
            events->count = count_vector;

            events->findI = findi_vector;
            events->func_on = func_on_vector;
            events->size = size_vector;
            events->sort = sort_vector;
            events->get_iterator = get_iterator_vector;
        }
    }
    push_memory_vactor(vector, capacity);

    return vector;
};
