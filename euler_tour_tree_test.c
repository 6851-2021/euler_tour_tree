#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// alternate tree representation with parent pointers and
// values array to test Euler Tour representation
typedef struct EulerTourTreeTest {
    k_t *parent;
    v_t *value;
    k_t n;
} EulerTourTreeTest;

// initialize tree with default values
EulerTourTreeTest* make_euler_tour_tree_test(s_t n) {
    EulerTourTreeTest* tree = malloc(sizeof(EulerTourTreeTest));
    tree->n = n;
    tree->parent = malloc(sizeof(v_t) * n);
    tree->value = malloc(sizeof(v_t) * n);
    for (k_t i = 0; i < n; i++) {
        tree->value[i] = i*10;
        tree->parent[i] = -1;
    }
    return tree;
}

// return whether u is an ancestor of v
bool is_ancestor(EulerTourTreeTest* tree, k_t u, k_t v) {
    while (v != -1) {
        if (u == v) {
            return true;
        }
        v = tree->parent[v];
    }
    return false;
}

// return root of tree containing node v
k_t find_root_test(EulerTourTreeTest* tree, k_t v) {
    while (tree->parent[v] != -1) {
        v = tree->parent[v];
    }
    return v;
}

// cut the subtree rooted at v from the rest of the tree
void cut_test(EulerTourTreeTest* tree, k_t v) {
    tree->parent[v] = -1;
}

// insert u as a child of v
void link_test(EulerTourTreeTest* tree, k_t u, k_t v) {
    tree->parent[u] = v;
}

// check whether nodes u and v are connected
bool connectivity_test(EulerTourTreeTest* tree, k_t u, k_t v) {
    return find_root_test(tree, u) == find_root_test(tree, v);
}

// update the value stored at node v
void point_update_test(EulerTourTreeTest* tree, k_t v, v_t new_value) {
    tree->value[v] = new_value;
}

// support subtree value incrementation updates
void subtree_update_test(EulerTourTreeTest* tree, k_t v, v_t delta) {
    tree->value[v] += delta;
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i] == v) {
            subtree_update_test(tree, i, delta);
        }
    }
}

// compute min of the nodes in the subtree rooted at v
v_t subtree_aggregate_min_test(EulerTourTreeTest* tree, k_t v) {
    v_t min_val = tree->value[v];
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i] == v) {
            min_val = MIN(min_val, subtree_aggregate_min_test(tree, i));
        }
    }
    return min_val;
}

// compute max of the nodes in the subtree rooted at v
v_t subtree_aggregate_max_test(EulerTourTreeTest* tree, k_t v) {
    v_t max_val = tree->value[v];
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i] == v) {
            max_val = MAX(max_val, subtree_aggregate_max_test(tree, i));
        }
    }
    return max_val;
}

// compute sum of the nodes in the subtree rooted at v
v_t subtree_aggregate_sum_test(EulerTourTreeTest* tree, k_t v) {
    v_t sum = tree->value[v];
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i] == v) {
            sum += subtree_aggregate_sum_test(tree, i);
        }
    }
    return sum;
}

// compute size of the subtree rooted at v
v_t subtree_aggregate_size_test(EulerTourTreeTest* tree, k_t v) {
    v_t count = 1;
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i] == v) {
            count += subtree_aggregate_size_test(tree, i);
        }
    }
    return count;
}

int main() {
    int n = 10000;
    EulerTourTree* tree = make_euler_tour_tree(n);
    EulerTourTreeTest* tree_test = make_euler_tour_tree_test(n);
    srand(32492341);
    for (int i = 0; i < 1000; i++) {
        int op = rand() % 9;

        if (op > 5) {
            op = 1;
        }

        // scanf("%d\n", &op);
        if (op == 0) {
            for (int i = 0; i < n; i++) {
                assert(find_root(tree, i) == find_root_test(tree_test, i));
            }
        } else if (op == 1) {
            int i = rand() % n;
            for (int j = 0; j < n; j++) {
                if (tree_test->parent[(j+i)%n] == -1) {
                    k_t par = rand() % ((j+i)%n);
                    link(tree, (j+i)%n, par);
                    link_test(tree_test, (j+i)%n, par);
                    break;
                }
            }
        } else if (op == 2) {
            int i = rand() % n;
            for (int j = 0; j < n; j++) {
                if (tree_test->parent[(j+i)%n] != -1) {
                    cut(tree, (j+i)%n);
                    cut_test(tree_test, (j+i)%n);
                    break;
                }
            }
        } else if (op == 3) {
            for (int i = 0; i < n; i++) {
                assert(subtree_aggregate_min(tree, i) == subtree_aggregate_min_test(tree_test, i));
                assert(subtree_aggregate_max(tree, i) == subtree_aggregate_max_test(tree_test, i));
                assert(subtree_aggregate_sum(tree, i) == subtree_aggregate_sum_test(tree_test, i));
                assert(subtree_aggregate_size(tree, i) == subtree_aggregate_size_test(tree_test, i));
            }
        } else if (op == 4) {
            int v = rand() % n;
            int val = rand() % 1000000;
            point_update(tree, v, val);
            point_update_test(tree_test, v, val);
        } else if (op == 5) {
            int v = rand() % n;
            int val = rand() % 100000;
            subtree_update(tree, v, val);
            subtree_update_test(tree_test, v, val);
        }
    }
}