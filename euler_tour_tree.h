#include "splay_tree.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct EulerTourTree {
    Node** visits;
    s_t n;
} EulerTourTree;

EulerTourTree* make_euler_tour_tree(s_t n);

k_t find_root(EulerTourTree* tree, k_t v);

void cut(EulerTourTree* tree, k_t v);

void link(EulerTourTree* tree, k_t u, k_t v);

#ifdef POINT_UPDATE
void point_update(EulerTourTree* tree, k_t v, v_t new_value);
#endif

#ifdef SUBTREE_INCREMENT
void subtree_update(EulerTourTree* tree, k_t v, v_t delta);
#endif

bool connectivity(EulerTourTree* tree, k_t u, k_t v);

#ifdef SUBTREE_MIN
v_t subtree_aggregate_min(EulerTourTree* tree, k_t k);
#endif

#ifdef SUBTREE_MAX
v_t subtree_aggregate_max(EulerTourTree* tree, k_t k);
#endif

#ifdef SUBTREE_SUM
v_t subtree_aggregate_sum(EulerTourTree* tree, k_t k);
#endif

#ifdef SUBTREE_SIZE
v_t subtree_aggregate_size(EulerTourTree* tree, k_t k);
#endif