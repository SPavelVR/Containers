#ifndef __ITERATOR_H
#define __ITERATOR_H

typedef struct Iterator {
    void* begin;
    void* end;
    void* iter;
    void (*pass)(struct Iterator*);
    void* (*get_data)(struct Iterator*);
} Iterator;

Iterator* init_iterator(void* begin, void* end, void (*pass)(struct Iterator*), void* (*get_data)(struct Iterator*));

void pass_iterator(Iterator* );

#define for_iterator(__iterator)    \
for ((__iterator)->iter = (__iterator)->begin; (__iterator)->iter != NULL; pass_iterator((__iterator)))

#endif