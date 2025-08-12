#include "..\include\node.h"
#include <stdlib.h>

void push_memory_node(Node* node, size_t capacity) {
    if (node == NULL || capacity == 0) return ;
    while (node->right != NULL) node = node->right;

    for (size_t i = 0; i < capacity; i++)
    {
        node->right = gen_node(node, NULL);
        node = node->right;
    }    
}


Node* init_node(size_t capacity) {
    Node* node = malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->data = NULL;
    node->left = NULL;
    node->right = NULL;
    if (capacity > 0) push_memory_node(node, capacity - 1);
    return node;
};
Node* gen_node(Node* left, Node* right) {
    Node* node = init_node(0);

    if (left != NULL)
        left->right = node;
    
    if (right != NULL)
        right->left = node;

    node->left = left;
    node->right = right;
    return node;
};
void conect_node(Node* left, Node* center, Node* right) {
    if (center == NULL) return ;
    if (left != NULL) left->right = center;
    if (right != NULL) right->left = center;

    center->left = left;
    center->right = right;
};

void free_node(Node* node) {
    if (node == NULL) return    ;
    while (node->right != NULL) node = node->right;

    while (node->left != NULL) {
        node = node->left;
        free(node->right);
    }
    free(node);
};

NodeOne* init_node_one() {
    NodeOne* nd = malloc(sizeof(NodeOne));
    if (nd == NULL) return NULL;
    nd->data = NULL;
    nd->right = NULL;
    return nd;
};

NodeOne* gen_next_node_one(NodeOne* next) {
    NodeOne* nd = malloc(sizeof(NodeOne));
    if (nd == NULL) return NULL;
    nd->data = NULL;
    nd->right = next;
    return nd;
};

NodeOne* gen_in_node_one(NodeOne* trouw) {
    NodeOne* nd = malloc(sizeof(NodeOne));
    if (nd == NULL) return NULL;
    nd->data = NULL;
    nd->right = trouw->right;
    trouw->right = nd;
    return nd;
};

void free_node_one(NodeOne* node) {
    while (node != NULL) {
        NodeOne* copy = node->right;
        free(node);
        node = copy;
    }
};