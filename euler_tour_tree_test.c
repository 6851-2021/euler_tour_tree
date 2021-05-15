#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct EulerTourTreeTest {
    bool **parent;
    bool *has_parent;
    v_t *value;
    k_t n;
} EulerTourTreeTest;

EulerTourTreeTest* make_euler_tour_tree_test(size_t n) {
    EulerTourTreeTest* tree = malloc(sizeof(EulerTourTreeTest));
    tree->n = n;
    tree->parent = malloc(sizeof(bool*) * n);
    tree->has_parent = malloc(sizeof(bool) * n);
    tree->value = malloc(sizeof(v_t) * n);
    for (k_t i = 0; i < n; i++) {
        tree->value = 0;
        tree->has_parent = false;
        tree->parent[i] = malloc(sizeof(bool) * n);
        for (k_t j = 0; j < n; j++) {
            tree->parent[i][j] = false;
        }
    }
    return tree;
}

k_t find_root_test(EulerTourTreeTest* tree, k_t v) {
    bool found_parent = false;
    do {
        for (k_t i = 0; i < tree->n; i++) {
            if (tree->parent[v][i]) {
                v = i;
                found_parent = true;
                break;
            }
        }
    } while (found_parent);
    return v;
}

void cut_test(EulerTourTreeTest* tree, k_t v) {
    tree->has_parent[v] = false;
    for (k_t i = 0; i < tree->n; i++) {
        tree->parent[v][i] = false;
    }
}

void link_test(EulerTourTreeTest* tree, k_t u, k_t v) {
    tree->has_parent[u] = true;
    tree->parent[u][v] = true;    
}

bool connectivity_test(EulerTourTreeTest* tree, k_t u, k_t v) {
    return find_root_test(tree, u) == find_root_test(tree, v);
}

void point_update_test(EulerTourTreeTest* tree, k_t v, v_t new_value) {
    tree->value[v] = new_value;
}

void subtree_update_test(EulerTourTreeTest* tree, k_t v, v_t delta) {
    tree->value[v] += delta;
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i][v]) {
            subtree_update_test(tree, i, delta);
        }
    }
}

v_t subtree_aggregate_min_test(EulerTourTreeTest* tree, k_t v) {
    v_t min_val = tree->value[v];
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i][v]) {
            min_val = MIN(min_val, subtree_aggregate_min_test(tree, i));
        }
    }
    return min_val;
}

v_t subtree_aggregate_max_test(EulerTourTreeTest* tree, k_t v) {
    v_t max_val = tree->value[v];
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i][v]) {
            max_val = MAX(max_val, subtree_aggregate_max_test(tree, i));
        }
    }
    return max_val;
}

v_t subtree_aggregate_sum_test(EulerTourTreeTest* tree, k_t v) {
    v_t sum = tree->value[v];
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i][v]) {
            sum += subtree_aggregate_sum_test(tree, i);
        }
    }
    return sum;
}

v_t subtree_aggregate_size_test(EulerTourTreeTest* tree, k_t v) {
    v_t count = 1;
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i][v]) {
            count += subtree_aggregate_size_test(tree, i);
        }
    }
    return count;
}

void print_node(EulerTourTree* tree, k_t v) {
    printf("min %ld, max %ld, sum %ld, size %ld\n", subtree_aggregate_min(tree, v), subtree_aggregate_max(tree, v), subtree_aggregate_sum(tree, v), subtree_aggregate_size(tree, v));
}

int main() {
    int n = 100;
    EulerTourTree* tree = make_euler_tour_tree(n);
    EulerTourTreeTest* tree_test = make_euler_tour_tree_test(n);
    srand(32492341);
    while(true) {
        
        int op;
        scanf("%d\n", &op);
        if (op == 0) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    assert(connectivity(tree, i, j) == connectivity_test(tree_test, i, j));
                }
            }
        } else if (op == 1) {
            int i = rand() % n;
            for (int j = i; j < n; j++) {
                if (!tree_test->has_parent[i]) {
                    k_t par = rand() % n;
                    link(tree, j, par);
                    link_test(tree_test, j, par);
                    goto next_loop;
                }
            }
            for (int j = 0; j < i; j++) {
                if (!tree_test->has_parent[i]) {
                    k_t par = rand() % n;
                    link(tree, j, par);
                    link_test(tree_test, j, par);
                    goto next_loop;
                }
            }
        } else if (op == 2) {
            int i = rand() % n;
            for (int j = i; j < n; j++) {
                if (tree_test->has_parent[i]) {
                    cut(tree, i);
                    cut_test(tree_test, i);
                    goto next_loop;
                }
            }
            for (int j = 0; j < i; j++) {
                if (tree_test->has_parent[i]) {
                    cut(tree, i);
                    cut_test(tree_test, i);
                    goto next_loop;
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
        next_loop:
            op = op;
    }

}