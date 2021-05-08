#include "splay_tree.h"
#include <stdint.h>

typedef struct EulerTourTree {
    Node** visits;
    size_t n;
} EulerTourTree;

EulerTourTree* make_euler_tour_tree(size_t n);

k_t find_root(EulerTourTree* tree, k_t k);

void cut(EulerTourTree* tree, k_t k);

void link(EulerTourTree* tree, k_t v, k_t w);

bool connectivity(EulerTourTree* tree, k_t v, k_t w);

v_t subtree_aggregate_min(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_max(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_sum(EulerTourTree* tree, k_t k);

v_t subtree_aggregate_size(EulerTourTree* tree, k_t k);