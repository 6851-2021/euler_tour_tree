#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

EulerTourTree* make_euler_tour_tree(size_t n) {
    EulerTourTree* tree = malloc(sizeof(EulerTourTree));
    tree->n = n;
    tree->visits = malloc(sizeof(Node*) * n * 2);
    for (k_t i = 0; i < n; ++i) {
        Node* node = make_node(i, 0);
        tree->visits[2*i] = node;
        tree->visits[2*i+1] = node;
    }
    return tree;
}

k_t find_root(EulerTourTree* tree, k_t v) {
    return find_min(tree->visits[2*v])->key;
}

void cut(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];
    Node* left = split_left(first);
    Node* right = split_right(last);

    Node* redundant = find_min(right);
    splay(redundant);

    // cut redundant node
    redundant->right->parent = NULL;

    // reassign last pointer if necessary
    if (tree->visits[2*redundant->key+1] == redundant) {
        tree->visits[2*redundant->key+1] = find_max(left);
    }

    merge(left, redundant->right);
    free(redundant);
}

void link(EulerTourTree* tree, k_t u, k_t v) {
    // insert u as a child of v
    Node* left = split_left(tree->visits[2*v+1]);
    Node* new_v = make_node(v, 0); // TODO this is a default value
    Node* root = merge(left, new_v);
    splay(tree->visits[2*u]);
    root = merge(root, tree->visits[2*u]);
    root = merge(root, tree->visits[2*v+1]);

    // if v was a leaf, need to update visits
    if (tree->visits[2*v] == tree->visits[2*v+1]) {
        tree->visits[2*v] = new_v;
    }
}

bool connectivity(EulerTourTree* tree, k_t u, k_t v) {
    return find_root(tree, u) == find_root(tree, v);
}

v_t subtree_aggregate_min(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_max(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_sum(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_size(EulerTourTree* tree, k_t k);

int main3() {
    EulerTourTree* tree = make_euler_tour_tree(10);
    bool connected = false;
    for (int i = 0; i < 10; i++) {
        for (int j = i+1; j < 10; j++) {
            if (connectivity(tree, i, j)) {
                connected = true;
                break;
            }
        }
    }
    printf("%d\n", connected);

    link(tree, 5, 4);
    link(tree, 6, 4);
    printf("%d\n", connectivity(tree, 4, 5));
    printf("%d\n", connectivity(tree, 4, 6));
    printf("%d\n", connectivity(tree, 6, 5));
    printf("%d\n", connectivity(tree, 1, 5));
}