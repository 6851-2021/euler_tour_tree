#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

EulerTourTree* make_euler_tour_tree(s_t n) {
    EulerTourTree* tree = malloc(sizeof(EulerTourTree));
    tree->n = n;
    tree->visits = malloc(sizeof(Node*) * n * 2);
    for (k_t i = 0; i < n; ++i) {
        Node* node = make_node(i, i*10, 1);
        tree->visits[2*i] = node;
        tree->visits[2*i+1] = node;
    }
    return tree;
}

k_t find_root(EulerTourTree* tree, k_t v) {
    Node* first_visit = tree->visits[2*v];
    splay(first_visit);
    Node* min_node = find_min(first_visit);
    splay(min_node);
    return min_node->key;
}

void cut(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];
    Node* left = split_left(first);
    Node* right = split_right(last);

    Node* redundant = find_min(right);
    splay(redundant);

    // cut redundant node
    if (redundant->right != NULL) {
        redundant->right->parent = NULL;
    }
    

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
    int is_first = (tree->visits[2*v] == tree->visits[2*v+1]);
    Node* new_v = make_node(v, tree->visits[2*v]->value, is_first);
    Node* root = merge(left, new_v);
    splay(tree->visits[2*u]);
    root = merge(root, tree->visits[2*u]);
    root = merge(root, tree->visits[2*v+1]);

    // if v was a leaf, need to update visits
    if (is_first) {
        update_node_start(tree->visits[2*v], 0);
        tree->visits[2*v] = new_v;
    }
    
}

bool connectivity(EulerTourTree* tree, k_t u, k_t v) {
    return find_root(tree, u) == find_root(tree, v);
}

#ifdef POINT_UPDATE
void point_update(EulerTourTree* tree, k_t v, v_t new_value) {
    update_node(tree->visits[2*v], new_value);
}
#endif

#ifdef SUBTREE_INCREMENT
void subtree_update(EulerTourTree* tree, k_t v, v_t delta) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    update_range(first, last, delta);
}
#endif

#ifdef SUBTREE_MIN
v_t subtree_aggregate_min(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).min;
}
#endif

#ifdef SUBTREE_MAX
v_t subtree_aggregate_max(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).max;
}
#endif

#ifdef SUBTREE_SUM
v_t subtree_aggregate_sum(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).sum;
}
#endif

#ifdef SUBTREE_SIZE
v_t subtree_aggregate_size(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).size;
}
#endif

void print_node(EulerTourTree* tree, k_t v) {
    printf("min %ld, max %ld, sum %ld, size %ld\n", subtree_aggregate_min(tree, v), subtree_aggregate_max(tree, v), subtree_aggregate_sum(tree, v), subtree_aggregate_size(tree, v));
}

int main2() {
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
    printf("connected? %d\n", connected);

    link(tree, 5, 4);
    link(tree, 6, 4);
    link(tree, 1, 3);
    link(tree, 3, 4);
    link(tree, 2, 4);
    
    printf("%d\n", connectivity(tree, 4, 5));
    printf("%d\n", connectivity(tree, 4, 6));
    printf("%d\n", connectivity(tree, 6, 5));
    printf("%d\n", connectivity(tree, 1, 5));

    subtree_update(tree, 4, 1);

    print_node(tree, 6);

    // printf("min %ld, max %ld, sum %ld, size %ld\n", subtree_aggregate_min(tree, 3), subtree_aggregate_max(tree, 3), subtree_aggregate_sum(tree, 3), subtree_aggregate_size(tree, 3));

    subtree_update(tree, 3, 2);

    printf("min is %ld\n", subtree_aggregate_min(tree, 4));

    printf("min %ld, max %ld, sum %ld, size %ld\n", subtree_aggregate_min(tree, 4), subtree_aggregate_max(tree, 4), subtree_aggregate_sum(tree, 4), subtree_aggregate_size(tree, 4));
}