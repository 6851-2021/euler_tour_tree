#include "euler_tour_tree.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N 20

bool all_connected(EulerTourTree* tree, size_t* nodes, size_t n) {
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            // printf("%lu %lu\n", nodes[i], nodes[j]);
            // printf("roots %lu %lu\n", find_root(tree, nodes[i]), find_root(tree, nodes[j]));
            if (!connectivity(tree, nodes[i], nodes[j])) {
                return false;
            }
        }
    }
    return true;
}

bool all_disconnected(EulerTourTree* tree, size_t* nodes, size_t n) {
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (connectivity(tree, nodes[i], nodes[j])) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    EulerTourTree* tree = make_euler_tour_tree(N);
    size_t* all_nodes = malloc(sizeof(size_t) * N);
    for (size_t i = 0; i < N; i++) {
        all_nodes[i] = i;
    }
    assert(all_disconnected(tree, all_nodes, N));

    for (size_t i = 0; i < N / 4; i++) {
        link(tree, 4*i+1, 4*i);
        link(tree, 4*i+2, 4*i+3);
        link(tree, 4*i+3, 4*i);
    }

    size_t four_nodes[4] = {0, 1, 2, 3};
    for (size_t i = 0; i < N / 4; i++) {
        assert(all_connected(tree, four_nodes, 4));
        for (int j = 0; j < 4; j++) {
            four_nodes[j] += 4;
        }
        assert(!connectivity(tree, 4*i, 4*i+5));
    }

    free(all_nodes);

    printf("passed\n");
}