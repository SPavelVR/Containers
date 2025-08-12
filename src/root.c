#include "../include/root.h"
#include <stdlib.h>

Root* gen_root(Root* parent) {
    Root* root = malloc(sizeof(Root));
    if (root == NULL) return NULL;
    root->left = NULL;
    root->right = NULL;
    root->data = NULL;
    root->parent = parent;
    root->size = 0;
    return root;
};

void free_root(Root* root) {
    if (root == NULL) return    ;
    if (root->parent != NULL) {
        if (root->parent->left == root) {
            root->parent->left = NULL;
        } else {
            root->parent->right = NULL;
        }
    }
    free_root(root->left);
    free_root(root->right);
    free(root);
};

int push_root(Root* root, void* data, int (*comp)(void* , void* )) {
    if (root == NULL || data == NULL || comp == NULL) return 0;
    
    if (root->data == NULL) {
        root->data = data;
        root->size = 1;
        if (root->right == NULL)
            root->right = gen_root(root);
        if (root->left == NULL)
            root->left = gen_root(root);
        return 1;
    }
    
    if (root->parent == NULL && find_root(root, data, comp) == 1) return 0;
    
    int c = comp(root->data, data);
    size_t r = size_root(root->right);
    size_t l = size_root(root->left);
    
    if (c > 0 && l <= r) {
        root = root->left;
        int res = push_root(root, data, comp);
        if (res > 0) root->parent->size++;
        return res;
    }
    
    if (c < 0 && l >= r) {
        root = root->right;
        int res = push_root(root, data, comp);
        if (res > 0) root->parent->size++;
        return res;
    }
    
    int res = 0;
    if (c > 0 && l > r) {
        void* oldData = root->data;
        root->data = data;
        res = push_root(root->right, oldData, comp);
        if (res > 0) root->size++;
        
        c = comp(get_max_root(root->left), root->data);
        if (c > 0) {
            void* newData = pop_max_root(root->left);
            root->data = newData;
            push_root(root->left, data, comp);
        }
    }
    
    if (c < 0 && l < r) {
        void* oldData = root->data;
        root->data = data;
        res = push_root(root->left, oldData, comp);
        if (res > 0) root->size++;
        
        c = comp(get_min_root(root->right), root->data);
        if (c < 0) {
            void* newData = pop_min_root(root->right);
            root->data = newData;
            push_root(root->right, data, comp);
        }
    }
    return res;
};

void* get_root(Root* root) {
    if (root == NULL) return NULL;
    return root->data;
};
void* get_max_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    if (root->right != NULL && root->right->data != NULL) 
        return get_max_root(root->right);
    return root->data;
};
void* get_min_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    if (root->left != NULL && root->left->data != NULL) 
        return get_min_root(root->left);
    return root->data;
};

void* pop_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    void* data = root->data;
    size_t l = size_root(root->left);
    size_t r = size_root(root->right);
    if (l >= r && l != 0) {
        root->data = pop_max_root(root->left);
    }
    return data;
};
void* pop_max_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    if (root->right != NULL && root->right->data != NULL) {
        void* res = pop_max_root(root->right);
        if (res != NULL) root->size--;
        return res;
    }
    void* max = root->data;
    root->data = NULL;
    Root* left = root->left;
    root->left = NULL;
    
    if (root->parent != NULL && root->parent->left == root) {
        root->parent->left = left;
    } else if (root->parent != NULL && root->parent->right == root) {
        root->parent->right = left;
    }
    left->parent = root->parent;
    root->parent = NULL;
    
    free_root(root);
    
    return max;
};
void* pop_min_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    if (root->left != NULL && root->left->data != NULL) {
        void* res = pop_min_root(root->left);
        if (res != NULL) root->size--;
        return res;
    }
    void* min = root->data;
    root->data = NULL;
    Root* right = root->right;
    root->right = NULL;
    
    if (root->parent != NULL && root->parent->left == root) {
        root->parent->left = right;
    } else if (root->parent != NULL && root->parent->right == root) {
        root->parent->right = right;
    }
    right->parent = root->parent;
    root->parent = NULL;
    
    free_root(root);
    
    return min;
};

int find_root(Root* root, void* data, int (*comp)(void* , void* )) {
    if (root == NULL || root->data == NULL) return 0;
    int c = comp(root->data, data);
    while (c != 0 && root != NULL && root->data != NULL) {
        if (c > 0) {
            root = root->left;
        } else {
            root = root->right;
        }
        if (root->data == NULL) return 0;
        c = comp(root->data, data);
    }
    if (c == 0) return 1;
    return 0;
};

Root* max_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    if (root->right != NULL && root->right->data != NULL) 
        return max_root(root->right);
    return root;
};
Root* min_root(Root* root) {
    if (root == NULL || root->data == NULL) return NULL;
    if (root->left != NULL && root->left->data != NULL) 
        return min_root(root->left);
    return root;
};

void func_on_root(Root* root, void (*func)(void*)) {
    if (root == NULL || root->data == NULL || func == NULL) return  ;
    
    Root* start = min_root(root);
    Root* end = max_root(root);
    while (start != NULL) {
        func(start->data);
        if (start == end) {
            start = NULL;
            continue;
        }
        if (start->right != NULL && start->right->data != NULL) {
            start = min_root(start->right);
            continue;
        }
        while (start->parent != NULL && start->parent->right == start) start = start->parent;
        start = start->parent;
    }
    
};

void pass_iterator_root(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return  ;
    if (iter->iter == iter->end) {
        iter->iter = NULL;
        return  ;
    }
    Root* root = (Root*) iter->iter;
    if (root->right != NULL && root->right->data != NULL) {
            root = min_root(root->right);
    } else {
        while (root->parent != NULL && root->parent->right == root) root = root->parent;
        root = root->parent;
    }
    iter->iter = root;
};

void* get_iterator_root_data(Iterator* iter) {
    if (iter == NULL || iter->iter == NULL) return NULL;
    return ((Root*) iter->iter)->data;
}

Iterator* get_iterator_root(Root* root) {
    Iterator* iter = init_iterator(min_root(root), max_root(root), pass_iterator_root, get_iterator_root_data);
    return iter;
};

size_t size_root(Root* root) {
    if (root == NULL || root->data == NULL) return 0;
    return root->size;
};