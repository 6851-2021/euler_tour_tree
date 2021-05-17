#include "splay_tree.h"
#include <stdlib.h>
#include <stdio.h>

// initialize augmentations of node
void augment_node(Node* node) {
    Node* left = node->left;
    Node* right = node-> right;

    #ifdef SUBTREE_MIN
    node->augments.min = INF;
    #endif
    #ifdef SUBTREE_MAX
    node->augments.max = -INF;
    #endif
    #ifdef SUBTREE_SUM
    node->augments.sum = 0;
    #endif
    #ifdef SUBTREE_SIZE
    node->augments.size = 0;
    #endif
#
    if (node->is_start) {
        #ifdef SUBTREE_MIN
        node->augments.min = node->value;
        #endif
        #ifdef SUBTREE_MAX
        node->augments.max = node->value;
        #endif
        #ifdef SUBTREE_SUM
        node->augments.sum = node->value;
        #endif
        #ifdef SUBTREE_SIZE
        node->augments.size = 1;
        #endif
    }

    if (left != NULL) {
        #ifdef SUBTREE_MIN
        node->augments.min = MIN(left->augments.min, node->augments.min);
        #endif
        #ifdef SUBTREE_MAX
        node->augments.max = MAX(left->augments.max, node->augments.max);
        #endif
        #ifdef SUBTREE_SUM
        node->augments.sum += left->augments.sum;
        #endif
        #ifdef SUBTREE_SIZE
        node->augments.size += left->augments.size;
        #endif
    }
    if (right != NULL) {
        #ifdef SUBTREE_MIN
        node->augments.min = MIN(right->augments.min, node->augments.min);
        #endif
        #ifdef SUBTREE_MAX
        node->augments.max = MAX(right->augments.max, node->augments.max);
        #endif
        #ifdef SUBTREE_SUM
        node->augments.sum += right->augments.sum;
        #endif
        #ifdef SUBTREE_SIZE
        node->augments.size += right->augments.size;
        #endif
    }
}

// propagate augmentations from node
void propagate(Node* node) {
    #ifdef SUBTREE_INCREMENT
    if (node != NULL && node->augments.lazy != 0) {
        node->value += node->augments.lazy;
        #ifdef SUBTREE_MIN
        node->augments.min += node->augments.lazy;
        #endif
        #ifdef SUBTREE_MAX
        node->augments.max += node->augments.lazy;
        #endif
        #ifdef SUBTREE_SUM
        node->augments.sum += (node->augments.lazy) * (node->augments.size);
        #endif
        if (node->left != NULL) {
            node->left->augments.lazy += node->augments.lazy;
        }
        if (node->right != NULL) {
            node->right->augments.lazy += node->augments.lazy;
        }
        node->augments.lazy = 0;
    }
    #endif
}

// create a splay tree node
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

#ifdef POINT_UPDATE
// update node value
void update_node(Node* node, v_t new_value) {
    splay(node);
    node->value = new_value;
    augment_node(node);
}
#endif

#ifdef SUBTREE_INCREMENT
// increment the values of the nodes in a given range
void update_range(Node* start, Node* end, v_t delta) {
    // split out subtree
    Node* left = split_left(start);
    Node* right = split_right(end);

    // increment values
    end->augments.lazy += delta;
    propagate(end);

    // re-concatenate tree
    merge(left, merge(end, right));
}
#endif

// update whether node is the start node
void update_node_start(Node* node, int is_start) {
    splay(node);
    node->is_start = is_start;
    augment_node(node);
}

// do a range query
Augmentations query(Node* start, Node* end) {
    // split out subtree
    Node* left = split_left(start);
    Node* right = split_right(end);

    // find query value
    Augmentations ret = end->augments;

    // re-concatenate tree
    merge(left, merge(end, right));
    return ret;
}

// rotate node right (and up one level)
// precondition: node is parent's left child
// postcondition: parent is node's right child
void right_rotate(Node* node) {
    Node* parent = node->parent;

    // propagate augmented values
    propagate(parent);
    propagate(parent->left);
    propagate(parent->right);
    propagate(node->left);
    propagate(node->right);

    // updating pointers with grandparent node
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

// rotate node left (and up one level)
// precondition: node is parent's right child
// postcondition: parent is node's left child
void left_rotate(Node* node) {
    Node* parent = node->parent;

    // propagate augmented values
    propagate(parent);
    propagate(parent->left);
    propagate(parent->right);
    propagate(node->left);
    propagate(node->right);

    // updating pointers with grandparent node
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

// bring node to root using rotations
void splay(Node* node) {

    // rotate until node is at root
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

// return min of tree rooted at node
// precondition: node is at root
Node* find_min(Node* node) {
    // node must not be null
    if (node == NULL) {
        return NULL;
    }

    // walk left to min
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// return max of tree rooted at node
// precondition: node is at root
Node* find_max(Node* node) {
    // node must not be null
    if (node == NULL) {
        return NULL;
    }

    // walk right to max
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

// split tree, right of node
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

// split tree, left of node
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

// merge two trees
// precondition: node1 and node2 are roots of their respective trees
// postcondition: node1's tree is concatenated left of node2's tree
Node* merge(Node* node1, Node* node2) {

    // if either node is null, return the other
    if (node1 == NULL) {
        return node2;
    }
    if (node2 == NULL) {
        return node1;
    }

    // max of node1's tree becomes new root
    Node* root = find_max(node1);
    splay(root);

    // append node2's tree
    root->right = node2;
    node2->parent = root;

    augment_node(root);
    return root;
}

// print in-order traversal of tree rooted at node
void print(Node* node) {
    if (node != NULL) {
        print(node->left);
        printf("(%ld, %ld) ", node->key, node->value);
        printf("[%ld, %ld, %ld, %ld] ", node->augments.min, node->augments.max,
        node->augments.sum, node->augments.size);
        print(node->right);
    }
}
