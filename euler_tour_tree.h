#include "splay_tree.h"
#include <stdint.h>
#include <stdbool.h>

// Euler Tour representation of a tree
typedef struct EulerTourTree {
    Node** visits;      // visits is a length-2n array of Node pointers.
                        // visits[2i] and visits[2i+1] stores the pointers to the
                        // first and last visits of node i, respectively.

    s_t n;              // n = number of nodes

    // Note the distinction between nodes in the ETT (keyed by integers between 0 and n-1)
    // and splay tree Nodes (which store the Euler Tour traversal of the ETT nodes).

} EulerTourTree;

// create a set of n disconnected nodes
EulerTourTree* make_euler_tour_tree(s_t n);

// return the root of the tree that contains v
k_t find_root(EulerTourTree* tree, k_t v);

// cut the subtree rooted at v from the rest of the tree
void cut(EulerTourTree* tree, k_t v);

// insert u's subtree as a child of v
void link(EulerTourTree* tree, k_t u, k_t v);

// check whether nodes u and v are connected
bool connectivity(EulerTourTree* tree, k_t u, k_t v);

// return the value stored at node v
v_t get_value(EulerTourTree* tree, k_t v);

// below: optional augmentations (see augmentations.h)

#ifdef POINT_UPDATE
// update the value stored at node v
void point_update(EulerTourTree* tree, k_t v, v_t new_value);
#endif

#ifdef SUBTREE_INCREMENT
// support subtree value incrementation updates
void subtree_update(EulerTourTree* tree, k_t v, v_t delta);
#endif

#ifdef SUBTREE_MIN
// compute min of the nodes in the subtree rooted at k
v_t subtree_aggregate_min(EulerTourTree* tree, k_t k);
#endif

#ifdef SUBTREE_MAX
// compute max of the nodes in the subtree rooted at k
v_t subtree_aggregate_max(EulerTourTree* tree, k_t k);
#endif

#ifdef SUBTREE_SUM
// compute sum of the nodes in the subtree rooted at k
v_t subtree_aggregate_sum(EulerTourTree* tree, k_t k);
#endif

#ifdef SUBTREE_SIZE
// compute size of the subtree rooted at k
v_t subtree_aggregate_size(EulerTourTree* tree, k_t k);
#endif