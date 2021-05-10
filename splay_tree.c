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
        if (parent == gp->left) {
            gp->left = node;
        }
        else {
            gp->right = node;
        }
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
        if (parent == gp->right) {
            gp->right = node;
        }
        else {
            gp->left = node;
        }
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

Node* find_min(Node* node) {
    // node must not be null
    if (node == NULL) {
        return NULL;
    }
    while (node->parent != NULL) {
        node = node->parent;
    }
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node* find_max(Node* node) {
    // node must not be null
    if (node == NULL) {
        return NULL;
    }
    while (node->parent != NULL) {
        node = node->parent;
    }
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

Node* split_right(Node* node) {
    // node ends up on left side
    // returns the root of the other (right) tree
    // which might be null
    splay(node);
    Node* right = node->right;
    if (right != NULL) {
        right->parent = NULL;
        node->right = NULL;
    }
    return right;
}

Node* split_left(Node* node) {
    // node ends up on right side
    // returns the root of the other (left) tree
    // which might be null
    splay(node);
    Node* left = node->left;
    if (left != NULL) {
        left->parent = NULL;
        node->left = NULL;
    }
    return left;
}

Node* merge(Node* node1, Node* node2) {
    // node1 and node2 are roots of their respective trees
    if (node1 == NULL) {
        return node2;
    }
    Node* root = find_max(node1);
    splay(root);
    root->right = node2;
    node2->parent = root;
    return root;
}

void print(Node* node) {
    if (node != NULL) {
        print(node->left);
        printf("(%ld, %ld) ", node->key, node->value);
        print(node->right);
    }
}

int main2() {
    Node* node1 = make_node(1, 0);
    Node* node2 = make_node(2, 0);
    Node* node3 = make_node(3, 0);
    Node* node4 = make_node(4, 0);
    Node* node5 = make_node(5, 0);
    Node* node6 = make_node(6, 0);
    Node* node7 = make_node(7, 0);
    Node* node8 = make_node(8, 0);
    Node* root = merge(node1, node2);
    root = merge(root, node3);
    root = merge(root, node4);
    Node* root2 = merge(node5, node6);
    root2 = merge(root2, node7);
    root2 = merge(root2, node8);
    root = merge(root, root2);
    print(root);
    printf("\n");
    splay(node4);
    print(node4);
    printf("\n");
    splay(node8);
    print(node8);
    printf("\n");
    splay(node2);
    print(node2);
    printf("\n-----------------\n");

    splay(node5);
    print(node5);
    printf("\n");
    Node* right = split_right(node5);
    print(right);
    printf("\n");
    print(node5);
    printf("\n");

    root = merge(node5, right);
    print(root);
    printf("\n");
    return 0;
}
