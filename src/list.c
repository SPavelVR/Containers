
#include "..\include\list.h"
#include "stdlib.h"
#include "list.h"

static EventsList* events = NULL;

void free_list (List* list) {
    if (list == NULL) return    ;
    free_node(list->container_end);
    free(list);
};

int push_back_list(List* list, void* data) {
    
    if (list == NULL || data == NULL) return 0;

    if (list->container_end == NULL) {
        list->container_end = init_node();
        list->container_begin = list->container_end;
    } else {
        list->container_end = gen_node(list->container_end, NULL);
    }

    list->size++;
    list->container_end->data = data;
    return 1;
};

int push_front_list(List* list, void* data) {
    if (list == NULL || data == NULL) return 0;

    if (list->container_begin == NULL) {
        list->container_end = init_node();
        list->container_begin = list->container_end;
    } else {
        list->container_begin = gen_node(NULL, list->container_begin);
    }

    list->size++;
    list->container_begin->data = data;
    return 1;
};

int push_index_list(List* list, void* data, size_t index) {
    if (list == NULL || list->size < index || data == NULL) return 0;

    if (index == 0) return push_front_list(list, data);
    if (index == list->size) return push_back_list(list, data);

    Node* node = list->container_begin;
    size_t i = 0;
    while (i < index && node != NULL && node->data != NULL) {
        i++;
        node = node->right;
    }

    if (i == index && node != NULL) {
        list->size++;
        Node* new_node = gen_node(node->left, node);
        new_node->data = data;
        return 1;
    }

    return 0;
};

void* front_list(List* list) {
    if (list == NULL || list->container_begin == NULL) return NULL;
    return list->container_begin->data;
};

void* back_list(List* list) {
    if (list == NULL || list->container_end == NULL) return NULL;
    return list->container_end->data;
};

void* index_list(List* list, size_t index) {
    if (list == NULL || list->size <= index) return NULL;
    Node* node = list->container_begin;
    size_t i = 0;
    while (i < index && node != NULL && node->data != NULL) {
        i++;
        node = node->right;
    }
    if (i == index && node != NULL) return node->data;
    return NULL;
};

void* pop_front_list(List* list) {
    if (list == NULL || list->container_begin == NULL) return NULL;

    Node* node = list->container_begin;
    list->container_begin = node->right;

    if (node == list->container_end) 
        list->container_end = node->right;

    if (list->container_begin != NULL)
        list->container_begin->left = NULL;
    
    node->right = NULL;
    
    list->size--;
    void* data = node->data;
    free_node(node);
    return data;
};
void* pop_back_list(List* list) {
    if (list == NULL || list->container_begin == NULL) return NULL;

    Node* node = list->container_end;
    list->container_end = node->left;

    if (node == list->container_begin) 
        list->container_begin = node->left;

    if (list->container_end != NULL)
        list->container_end->right = NULL;
    
    node->left = NULL;
    
    list->size--;
    void* data = node->data;
    free_node(node);
    return data;
};
void* pop_index_list(List* list, size_t index) {
    if (list == NULL || list->size <= index) return NULL;

    if (index == 0) return pop_front_list(list);
    if (index == (list->size - 1)) return pop_back_list(list);

    Node* node = list->container_begin;
    size_t i = 0;

    while (i < index && node != NULL) {
        i++;
        node = node->right;
    }

    if (i == index && node != NULL && node->data != NULL) {

        void* data = node->data;

        Node* left = node->left;
        Node* right = node->right;

        node->left = NULL;
        node->right = NULL;

        free_node(node);

        if (left != NULL) left->right = right;
        if (right != NULL) right->left = left;

        list->size--;
        return data;
    }

    return NULL;
};

size_t count_list(List* list, void* data, int (*comp) (void*, void*)) {
    if (list == NULL || data == NULL) return 0;

    Node* node = list->container_begin;
    size_t count = 0;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) count++;
        node = node->right;
    }

    return count;
};

void* find_list(List* list, void* data, int (*comp) (void*, void*)) {
    if (list == NULL || data == NULL) return NULL;

    Node* node = list->container_begin;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) return node->data;
        node = node->right;
    }

    return NULL;
};

size_t findi_list(List* list, void* data, int (*comp) (void*, void*)) {
    if (list == NULL || data == NULL) return 0;

    Node* node = list->container_begin;
    size_t index = 0;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) return index;
        node = node->right;
        index++;
    }

    return index;
};

int add_list(List* list, void* data, int (*comp) (void*, void*)) {
    if (list == NULL || data == NULL) return 0;

    size_t ind = find_list(list, data, comp);

    if (ind >= list->size) return push_back_list(list, data);

    return 0;
}

size_t size_list(List* list) {
    if (list == NULL) return 0;
    return list->size;
};

void func_on_list(List* list, void (*func)(void*)) {
    if (list == NULL || list->container_begin == NULL || list->container_begin->data == NULL || func == NULL) return   ;

    Node* node = list->container_begin;
    while (node != NULL && node->data != NULL) {
        func(node->data);
        node = node->right;
    }
};

void sort_list(List* list, int (*comp)(void*, void*)) {
    if (list == NULL || list->size <= 1) return ;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) rcomp = list->comp;

    Node* node = list->container_begin;
    Node* start = list->container_begin;

    int flag = 0;

    while (node != NULL && node->right != NULL && node->right->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, node->right->data) > 0) {
            void* temp = node->data;
            node->data = node->right->data;
            node->right->data = temp;
            flag = 1;
        }

        node = node->right;

        if (flag && (node->right == NULL || node->right->data == NULL)) {
            node = start;
            flag = 0;
        }
    }
};

void pass_iterator_list(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return  ;
    
    iter->iter = (void*) ((Node*) iter->iter)->right;
}

void* get_data_iterator_list(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return NULL;

    return ((Node*) iter->iter)->data;
}

Iterator* get_iterator_list(List* list) {
    if (list == NULL || list->size == 0) return NULL;
    Iterator* iter = init_iterator(list->container_begin, list->container_end, pass_iterator_list, get_data_iterator_list);
    return iter;
};

void* remove_list (List* list, void* data, int (*comp) (void*, void*)) {
    if (list == NULL || data == NULL) return NULL;

    Node* node = list->container_begin;
    size_t index = 0;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) return pop_index_list(list, index);
        node = node->right;
        index++;
    }

    return NULL;
};

List* init_list(int (*comp) (void*, void*)) {
    List* list = malloc(sizeof(List));
    if (list == NULL) return NULL;

    list->container_begin = NULL;
    list->container_end = NULL;
    list->comp = comp;
    list->size = 0;
    if (events == NULL) {
        events = malloc(sizeof(EventsList));
        if (events != NULL) {
            events->free = free_list;

            events->back = back_list;
            events->front = front_list;
            events->index = index_list;

            events->push_back = push_back_list;
            events->push_front = push_front_list;
            events->push_index = push_index_list;

            events->pop_back = pop_back_list;
            events->pop_front = pop_front_list;
            events->pop_index = pop_index_list;

            events->add = add_list;
            events->remove = remove_list;
            events->find = find_list;

            events->count = count_list;
            events->findI = findi_list;
            events->func_on = func_on_list;
            events->size = size_list;
            events->sort = sort_list;
            events->get_iterator = get_iterator_list;
        }
    }
    list->events = events;
    return list;
}