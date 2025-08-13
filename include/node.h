#ifndef __NODE_H
#define __NODE_H

#include <stddef.h>

typedef struct Node {
    void* data;
    struct Node *right; 
    struct Node *left;
} Node;

typedef struct NodeOne {
    void* data;
    struct NodeOne *right; 
} NodeOne;

Node* init_node();
Node* gen_node(Node* left, Node* right);
void free_node(Node* node);

NodeOne* init_node_one();
NodeOne* gen_next_node_one(NodeOne* next);
NodeOne* gen_in_node_one(NodeOne* trouw);
void free_node_one(NodeOne* node);

#endif