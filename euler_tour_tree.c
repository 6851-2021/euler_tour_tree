#include "euler_tour_tree.h"
#include <stdint.h>

EulerTourTree* make_euler_tour_tree(size_t n) {
    EulerTourTree* tree = malloc(sizeof(EulerTourTree));
    tree->n = n;
    tree->visits = malloc(sizeof(Node*) * n * 2);
    // Node* nodes = malloc(sizeof(Node) * n);
    for (k_t i = 0; i < n; ++i) {
        Node* node = make_node(i, 0);
        tree->visits[2*i] = node;
        tree->visits[2*i+1] = node;
    }
    return tree;
}

k_t find_root(EulerTourTree* tree, k_t k) {
    return find_min(tree->visits[2*k])->key;
}

void cut(EulerTourTree* tree, k_t k) {

}

void link(EulerTourTree* tree, k_t v, k_t w);

bool connectivity(EulerTourTree* tree, k_t v, k_t w);

v_t subtree_aggregate_min(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_max(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_sum(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_size(EulerTourTree* tree, k_t k);