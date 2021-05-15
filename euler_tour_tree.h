#include "splay_tree.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct EulerTourTree {
    Node** visits;
    size_t n;
} EulerTourTree;

EulerTourTree* make_euler_tour_tree(size_t n);

k_t find_root(EulerTourTree* tree, k_t v);

void cut(EulerTourTree* tree, k_t v);

void link(EulerTourTree* tree, k_t u, k_t v);

void point_update(EulerTourTree* tree, k_t v, v_t new_value);

void subtree_update(EulerTourTree* tree, k_t v, v_t delta);

bool connectivity(EulerTourTree* tree, k_t u, k_t v);

v_t subtree_aggregate_min(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_max(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_sum(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_size(EulerTourTree* tree, k_t k);