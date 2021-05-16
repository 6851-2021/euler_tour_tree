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

EulerTourTreeTest* make_euler_tour_tree_test(s_t n) {
    EulerTourTreeTest* tree = malloc(sizeof(EulerTourTreeTest));
    tree->n = n;
    tree->parent = malloc(sizeof(bool*) * n);
    tree->has_parent = malloc(sizeof(bool) * n);
    tree->value = malloc(sizeof(v_t) * n);
    for (k_t i = 0; i < n; i++) {
        tree->value[i] = i*10;
        tree->has_parent[i] = false;
        tree->parent[i] = malloc(sizeof(bool) * n);
        for (k_t j = 0; j < n; j++) {
            tree->parent[i][j] = false;
        }
    }
    return tree;
}

bool is_ancestor(EulerTourTreeTest* tree, k_t u, k_t v) {
    while (tree->has_parent[v]) {
        if (u == v) {
            return true;
        }
        for (k_t i = 0; i < tree->n; i++) {
            if (tree->parent[v][i]) {
                v = i;
                break;
            }
        }
    }

    return u == v;
}

k_t find_root_test(EulerTourTreeTest* tree, k_t v) {
    while (tree->has_parent[v]) {
        for (k_t i = 0; i < tree->n; i++) {
            if (tree->parent[v][i]) {
                v = i;
                break;
            }
        }
    }
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

v_t subtree_aggregate_s_test(EulerTourTreeTest* tree, k_t v) {
    v_t count = 1;
    for (k_t i = 0; i < tree->n; i++) {
        if (tree->parent[i][v]) {
            count += subtree_aggregate_s_test(tree, i);
        }
    }
    return count;
}

int main() {
    int n = 1000;
    EulerTourTree* tree = make_euler_tour_tree(n);
    EulerTourTreeTest* tree_test = make_euler_tour_tree_test(n);
    srand(32492341);
    while(true) {
        int op = rand() % 9;

        if (op > 5) {
            op = 1;
        }
        
        // int op;
        // scanf("%d\n", &op);
        if (op == 0) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    assert(connectivity(tree, i, j) == connectivity_test(tree_test, i, j));
                }
            }
        } else if (op == 1) {
            int i = rand() % n;
            for (int j = 0; j < n; j++) {
                if (!tree_test->has_parent[i]) {
                    k_t par = rand() % n;
                    int counter = 0;
                    while (is_ancestor(tree_test, (j+i)%n, par) && counter < 100) {
                        par = rand() % n;
                        counter ++;
                    }
                    if (counter < 100) {
                        link(tree, (j+i)%n, par);
                        link_test(tree_test, (j+i)%n, par);
                    }
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
                assert(subtree_aggregate_size(tree, i) == subtree_aggregate_s_test(tree_test, i));
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