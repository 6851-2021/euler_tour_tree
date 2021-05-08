#include "splay_tree.h"
#include <stdlib.h>

Node* make_node(k_t key, v_t value) {
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

void right_rotate(Node* node) {
    Node* parent = node->parent;
    Node* gp = parent->parent;
    if (gp != NULL) {
        gp->left = node;
    }
    node->parent = gp;
    parent->parent = node;
    parent->left = node->right;
    if (parent->left != NULL) {
        parent->left->parent = parent;
    }
    node->right = parent;
}

void left_rotate(Node* node) {
    Node* parent = node->parent;
    Node* gp = parent->parent;
    if (gp != NULL) {
        gp->right = node;
    }
    node->parent = gp;
    parent->parent = node;
    parent->right = node->left;
    if (parent->right != NULL) {
        parent->right->parent = parent;
    }
    node->left = parent;
}

void splay(Node* node);

void print(Node* node);