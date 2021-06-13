#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// create the Euler Tour representation of a set of n disconnected nodes
EulerTourTree* make_euler_tour_tree(s_t n) {
    EulerTourTree* tree = malloc(sizeof(EulerTourTree));
    tree->n = n;

    // visits is a length-2n array of Node pointers.
    // visits[2i] and visits[2i+1] stores the pointers to the
    // first and last visits of node i, respectively.
    tree->visits = malloc(sizeof(Node*) * n * 2);

    // make n Nodes with default values
    for (k_t i = 0; i < n; ++i) {
        Node* node = make_node(i, 0, 1);
        tree->visits[2*i] = node;
        tree->visits[2*i+1] = node;
    }

    return tree;
}

void destroy_euler_tour_tree(EulerTourTree* tree) {
    
    s_t n = tree->n;
    bool* is_deleted = malloc(n * sizeof(bool));
    for (s_t i = 0; i < n; ++i) {
        is_deleted[i] = false;
    }

    for (k_t i = 0; i < n; ++i) {
        if (!is_deleted[i]) {
            Node* node = tree->visits[2*i];
            splay(node);
            delete_recursive_and_mark(node, is_deleted);
        }
    }

    free(is_deleted);
    free(tree->visits);
    free(tree);
}

// return the root of the tree that contains v
k_t find_root(EulerTourTree* tree, k_t v) {
    // return minimum element of the splay tree starting from first visit to v

    Node* first_visit = tree->visits[2*v];
    splay(first_visit);
    Node* min_node = find_min(first_visit);
    splay(min_node);
    return min_node->key;
}

// cut the subtree rooted at v from the rest of the tree
void cut(EulerTourTree* tree, k_t v) {

    // split splay tree before first and after last visit to v
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];
    Node* left = split_left(first);
    Node* right = split_right(last);

    // potential extra visitation
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

// insert u as a child of v
void link(EulerTourTree* tree, k_t u, k_t v) {

    // split before last visit to v
    Node* left = split_left(tree->visits[2*v+1]);

    // create new visit to v
    int is_first = (tree->visits[2*v] == tree->visits[2*v+1]);
    Node* new_v = make_node(v, tree->visits[2*v]->value, is_first);

    // concatenate visits in the correct order
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

// check whether nodes u and v are connected
bool connectivity(EulerTourTree* tree, k_t u, k_t v) {
    return find_root(tree, u) == find_root(tree, v);
}

// return the value stored at node v
v_t get_value(EulerTourTree* tree, k_t v) {
    return tree->visits[2*v]->value;
}

#ifdef POINT_UPDATE
// update the value stored at node v
void point_update(EulerTourTree* tree, k_t v, v_t new_value) {
    update_node(tree->visits[2*v], new_value);
}
#endif

#ifdef SUBTREE_INCREMENT
// support subtree value incrementation updates
void subtree_update(EulerTourTree* tree, k_t v, v_t delta) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    update_range(first, last, delta);
}
#endif

#ifdef SUBTREE_MIN
// compute min of the nodes in the subtree rooted at k
v_t subtree_aggregate_min(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).min;
}
#endif

#ifdef SUBTREE_MAX
// compute max of the nodes in the subtree rooted at k
v_t subtree_aggregate_max(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).max;
}
#endif

#ifdef SUBTREE_SUM
// compute sum of the nodes in the subtree rooted at k
v_t subtree_aggregate_sum(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).sum;
}
#endif

#ifdef SUBTREE_SIZE
// compute size of the subtree rooted at k
v_t subtree_aggregate_size(EulerTourTree* tree, k_t v) {
    Node* first = tree->visits[2*v];
    Node* last = tree->visits[2*v+1];

    return query(first, last).size;
}
#endif

// helper print
void print_node(EulerTourTree* tree, k_t v) {
    printf("min %ld, max %ld, sum %ld, size %ld\n", subtree_aggregate_min(tree, v), subtree_aggregate_max(tree, v), subtree_aggregate_sum(tree, v), subtree_aggregate_size(tree, v));
}