#include "splay_tree.h"
#include <stdlib.h>
#include <stdio.h>

void augment_node(Node* node) {
    Node* left = node->left;
    Node* right = node-> right;

    node->augments.min = INF;
    node->augments.max = -INF;

    node->augments.sum = 0;
    node->augments.size = 0;

    if (node->is_start) {
        node->augments.min = node->value;
        node->augments.max = node->value;

        node->augments.sum = node->value;
        node->augments.size = 1;
    }

    if (left != NULL) {
        node->augments.min = MIN(left->augments.min, node->augments.min);
        node->augments.max = MAX(left->augments.max, node->augments.max);
        node->augments.sum += left->augments.sum;
        node->augments.size += left->augments.size;
    }
    if (right != NULL) {
        node->augments.min = MIN(right->augments.min, node->augments.min);
        node->augments.max = MAX(right->augments.max, node->augments.max);
        node->augments.sum += right->augments.sum;
        node->augments.size += right->augments.size;
    }
}

void propagate(Node* node) {
    if (node != NULL && node->augments.lazy != 0) {
        node->value += node->augments.lazy;
        node->augments.min += node->augments.lazy;
        node->augments.max += node->augments.lazy;
        node->augments.sum += (node->augments.lazy) * (node->augments.size);

        if (node->left != NULL) {
            node->left->augments.lazy += node->augments.lazy;
        }

        if (node->right != NULL) {
            node->right->augments.lazy += node->augments.lazy;
        }

        node->augments.lazy = 0;
    }
    
}

Node* make_node(k_t key, v_t value, int is_start) {
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->is_start = is_start;

    augment_node(node);
    return node;
}

void update_range(Node* start, Node* end, v_t delta) {
    Node* left = split_left(start);
    Node* right = split_right(end);

    end->augments.lazy += delta;
    propagate(end);
    
    merge(left, merge(end, right));
}

void update_node(Node* node, v_t new_value) {
    splay(node);
    node->value = new_value;
    augment_node(node);
}

void update_node_start(Node* node, int is_start) {
    splay(node);
    node->is_start = is_start;
    augment_node(node);
}

Augmentations query(Node* start, Node* end) {
    Node* left = split_left(start);
    Node* right = split_right(end);
    // printf("end is %p\n", end);
    // printf("left and right are %p, %p\n", left, right);
    Augmentations ret = end->augments;
    merge(left, merge(end, right));
    return ret;
}


void right_rotate(Node* node) {
    Node* parent = node->parent;
    propagate(parent);
    propagate(parent->left);
    propagate(parent->right);
    propagate(node->left);
    propagate(node->right);

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

    augment_node(parent);
    augment_node(node);
}

void left_rotate(Node* node) {
    Node* parent = node->parent;

    propagate(parent);
    propagate(parent->left);
    propagate(parent->right);
    propagate(node->left);
    propagate(node->right);

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

    augment_node(parent);
    augment_node(node);
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

    augment_node(node);
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

    augment_node(node);
    return left;
}

Node* merge(Node* node1, Node* node2) {
    // printf("merging nodes: %p, %p\n", node1, node2);
    // node1 and node2 are roots of their respective trees
    if (node1 == NULL) {
        return node2;
    }
    if (node2 == NULL) {
        return node1;
    }
    Node* root = find_max(node1);
    splay(root);
    root->right = node2;
    node2->parent = root;

    augment_node(root);
    return root;
}

void print(Node* node) {
    if (node != NULL) {
        print(node->left);
        printf("(%ld, %ld) ", node->key, node->value);
        printf("[%ld, %ld, %ld, %ld] ", node->augments.min, node->augments.max,
        node->augments.sum, node->augments.size);
        print(node->right);
    }
}

int main3() {
    Node* node1 = make_node(1, 10, 1);
    Node* node2 = make_node(2, 20, 1);
    Node* node3 = make_node(3, 30, 1);
    Node* node4 = make_node(4, 40, 1);
    Node* node5 = make_node(5, 50, 1);
    Node* node6 = make_node(6, 60, 1);
    Node* node7 = make_node(7, 70, 1);
    Node* node8 = make_node(8, 80, 1);
    Node* root = merge(node1, node2);
    root = merge(root, node3);
    root = merge(root, node4);
    Node* root2 = merge(node5, node6);
    root2 = merge(root2, node7);
    root2 = merge(root2, node8);
    root = merge(root, root2);
    print(root);
    printf("\nmin and max and sum and size values:\n");
    printf("%ld, %ld, %ld, %ld\n", root->augments.min, root->augments.max, root->augments.sum, root->augments.size);
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
    update_range(node1, node8, 5);
    printf("query below\n");
    Augmentations aug = query(node3, node7);
    printf("min, max, sum, size: %ld, %ld, %ld, %ld\n", aug.min, aug.max, aug.sum, aug.size);
    print(node5);
    printf("\n");
    Node* right = split_right(node5);
    print(right);
    printf("\n%ld, %ld, %ld, %ld\n", right->augments.min, right->augments.max, right->augments.sum, right->augments.size);
    printf("\n");
    print(node5);
    printf("\n");

    root = merge(node5, right);
    print(root);
    printf("\n");
    return 0;
}
