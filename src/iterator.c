#include "../include/iterator.h"
#include <stdlib.h>


Iterator* init_iterator(void* begin, void* end, void (*pass)(struct Iterator*), void* (*get_data)(struct Iterator*)) {
    Iterator* iter = malloc(sizeof(Iterator));
    if (iter == NULL) return NULL;
    iter->begin = begin;
    iter->iter = begin;
    iter->end = end;
    iter->pass = pass;
    iter->get_data = get_data;
    return iter;
};

void pass_iterator(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) 
        return ;
    if (iter->iter == iter->end) {
        iter->iter = NULL;
        return  ;
    }
    iter->pass(iter);
};