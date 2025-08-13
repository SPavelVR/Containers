#include "..\include\linkedList.h"
#include "stdlib.h"

static EventsLinkedList* events = NULL;

void free_linked_list (LinkedList* linked_list) {
    if (linked_list == NULL) return    ;
    free_node_one(linked_list->container);
    free(linked_list);
};

int push_front_linked_list(LinkedList* linked_list, void* data) {
    if (linked_list == NULL || data == NULL) return 0;

    NodeOne* node = linked_list->container;

    if (node == NULL) {
        linked_list->container = init_node_one();
        linked_list->container->data = data;
    }
    else if (node->data == NULL) {
        node->data = data;
    }
    else {
        node = gen_next_node_one(node);
        node->data = data;
        linked_list->container = node;
    }
    linked_list->size++;
    return 1;
};

int push_back_linked_list(LinkedList* linked_list, void* data) {
    if (linked_list == NULL || data == NULL) return 0;
    
    if (linked_list->size == 0) return push_front_linked_list(linked_list, data);

    NodeOne* node = linked_list->container;

    while (node->right != NULL && node->data != NULL) node = node->right;

    if (node->data == NULL) node->data = data;
    else if (node->right == NULL) {
        node->right = init_node_one();
        node->right->data = data;
    }
    linked_list->size++;
    return 1;
};

int push_index_linked_list(LinkedList* linked_list, void* data, size_t index) {
    if (linked_list == NULL || linked_list->size < index || data == NULL) return 0;
    if (index == 0) return push_front_linked_list(linked_list, data);
    if (index == linked_list->size - 1) return push_back_linked_list(linked_list, data);

    NodeOne* node = linked_list->container;
    size_t i = 1;
    while (i < index && node != NULL && node->data != NULL) {
        i++;
        node = node->right;
    }

    if (i == index && node != NULL) {
        linked_list->size++;
        NodeOne* new_node = gen_in_node_one(node);
        new_node->data = data;
        return 1;
    }

    return 0;
};

void* front_linked_list(LinkedList* linked_list) {
    if (linked_list == NULL || linked_list->container == NULL) return NULL;
    return linked_list->container->data;
};
void* back_linked_list(LinkedList* linked_list) {
    if (linked_list == NULL || linked_list->container == NULL || linked_list->container->data == NULL) return NULL;
    NodeOne* node = linked_list->container;
    while (node->right != NULL && node->right->data != NULL) {
        node = node->right;
    }
    return node->data;
};
void* index_linked_list(LinkedList* linked_list, size_t index) {
    if (linked_list == NULL || linked_list->size <= index) return NULL;
    NodeOne* node = linked_list->container;
    size_t i = 0;
    while (i < index && node != NULL && node->data != NULL) {
        i++;
        node = node->right;
    }
    if (i == index) return node->data;
    return NULL;
};

void* pop_front_linked_list(LinkedList* linked_list) {
    if (linked_list == NULL || linked_list->container == NULL || linked_list->container->data == NULL || linked_list->size == 0) return NULL;
    
    void* data = linked_list->container->data;

    NodeOne* right = linked_list->container->right;

    if (right != NULL) {
        linked_list->container->right = right->right;
        right->right = NULL;
        linked_list->container->data = right->data;
        free_node_one(right);
    } else {
        linked_list->container->data = NULL;
    }

    linked_list->size--;
    return data;
};

void* pop_index_linked_list(LinkedList* linked_list, size_t index) {
    if (linked_list == NULL || linked_list->size <= index) return NULL;

    if (index == 0) return pop_front_linked_list(linked_list);

    NodeOne* node = linked_list->container;
    size_t i = 0;

    while (i < index && node != NULL) {
        i++;
        node = node->right;
    }

    if (i == index && node != NULL) {

        void* data = node->data;

        NodeOne* right = node->right;

        if (right != NULL) {
            node->right = right->right;
            right->right = NULL;
            node->data = right->data;
            free_node_one(right);
        } else {
            node->data = NULL;
        }

        linked_list->size--;
        return data;
    }
    return NULL;
};

void* pop_back_linked_list(LinkedList* linked_list) {
    if (linked_list == NULL || linked_list->size == 0) return NULL;

    return pop_index_linked_list(linked_list, linked_list->size - 1);
};

size_t find_linked_list(LinkedList* linked_list, void* data, int (*comp) (void*, void*)) {
    if (linked_list == NULL || data == NULL) return 0;

    NodeOne* node = linked_list->container;
    size_t index = 0;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = linked_list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) return index;
        node = node->right;
        index++;
    }

    return (index);
};

size_t count_linked_list(LinkedList* linked_list, void* data, int (*comp) (void*, void*)) {
    if (linked_list == NULL || data == NULL) return 0;

    NodeOne* node = linked_list->container;
    size_t count = 0;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = linked_list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) count++;
        node = node->right;
    }

    return count;
};

size_t size_linked_list(LinkedList* linked_list) {
    if (linked_list == NULL) return 0;
    return linked_list->size;
};

void func_on_linked_list(LinkedList* linked_list, void (*func)(void*)) {
    if (linked_list == NULL || linked_list->container == NULL || linked_list->container->data == NULL || func == NULL) return   ;

    NodeOne* node = linked_list->container;
    while (node != NULL && node->data != NULL) {
        func(node->data);
        node = node->right;
    }
};
void sort_linked_list(LinkedList* linked_list, int (*comp)(void*, void*)) {
    if (linked_list == NULL || linked_list->size <= 1) return ;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) rcomp = linked_list->comp;

    NodeOne* node = linked_list->container;
    NodeOne* start = linked_list->container;

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

void pass_iterator_linked_list(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return  ;
    
    iter->iter = (void*) ((NodeOne*) iter->iter)->right;
}

void* get_data_iterator_linked_list(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return NULL;
    return ((NodeOne*) iter->iter)->data;
}

Iterator* get_iterator_linked_list(LinkedList* linked_list) {
    if (linked_list == NULL || linked_list->size == 0) return NULL;

    NodeOne* end = linked_list->container;
    while (end->right != NULL && end->right->data != NULL) {
        end = end->right;
    }
    Iterator* iter = init_iterator(linked_list->container, end, pass_iterator_linked_list, get_data_iterator_linked_list);
    return iter;
};

void* remove_linked_list (LinkedList* linked_list, void* data, int (*comp) (void*, void*)) {
    if (linked_list == NULL || data == NULL) return NULL;

    NodeOne* node = linked_list->container;
    size_t index = 0;

    int (*rcomp)(void*, void*) = comp;

    if (rcomp == NULL) {
        rcomp = linked_list->comp;
    }

    while (node != NULL && node->data != NULL && rcomp != NULL) {
        if (rcomp(node->data, data) == 0) {
            linked_list->size--;
            NodeOne* right = node->right;

            void* res = node->data;

            if (right != NULL) {
                node->right = right->right;
                node->data = right->data;
                right->right = NULL;
                free_node_one(right);
            } else node->data = NULL;

            return res;
        }
        node = node->right;
        index++;
    }

    return NULL;
};

int add_linked_list (LinkedList* linked_list, void* data, int (*comp) (void*, void*)) {
    if (linked_list == NULL || data == NULL) return 0;

    size_t ind = find_linked_list(linked_list, data, comp);

    if (ind >= size_linked_list(linked_list)) {
        return push_front_linked_list(linked_list, data);
    }

    return 0;
};

LinkedList* init_linked_list(int (*comp) (void*, void*)) {
    LinkedList* linked_list = malloc(sizeof(LinkedList));

    if (linked_list == NULL)return NULL;

    linked_list->container = NULL;
    linked_list->comp = comp;
    linked_list->size = 0;

    if (events == NULL) {
        events = malloc(sizeof(EventsLinkedList));
        if (events != NULL) {
            events->free = free_linked_list;

            events->back = back_linked_list;
            events->front = front_linked_list;
            events->index = index_linked_list;

            events->push_back = push_back_linked_list;
            events->push_front = push_front_linked_list;
            events->push_index = push_index_linked_list;

            events->pop_back = pop_back_linked_list;
            events->pop_front = pop_front_linked_list;
            events->pop_index = pop_index_linked_list;

            events->add = add_linked_list;
            events->remove = remove_linked_list;
            events->count = count_linked_list;
            
            events->find = find_linked_list;
            events->func_on = func_on_linked_list;
            events->size = size_linked_list;
            events->sort = sort_linked_list;
            events->get_iterator = get_iterator_linked_list;
        }
    }
    linked_list->events = events;
    return linked_list;
}