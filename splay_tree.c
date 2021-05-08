#include "splay_tree.h"
#include <stdlib.h>
#include <stdio.h>

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

void splay(Node* node) {
    while (node->parent) {
        Node* parent = node->parent;
        Node* gp = parent->parent;
        if (gp == NULL) {
            // zig step
            if (node == parent->left) {
                right_rotate(node);
            } else {
                left_rotate(node);
            }
            break;
        }
        if (node == parent->left) {
            if (parent == gp->left) {
                // zig-zig
                right_rotate(parent);
                right_rotate(node);
            } else {
                // zig-zag
                right_rotate(node);
                left_rotate(node);
            }
        } else {
            if (parent == gp->left) {
                // zig-zag
                left_rotate(node);
                right_rotate(node);
            } else {
                // zig-zig
                left_rotate(parent);
                left_rotate(node);
            }
        }
    }
}

void print(Node* node) {
    if (node != NULL) {
        print(node->left);
        printf("(%ld, %ld)", node->key, node->value);
        print(node->right);
    }
}

int main() {
    Node* node = make_node(10, 15);
    print(node);
    printf("\n");
    return 0;
}