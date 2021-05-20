#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Reads in a file created by benchmark_gen, performs the listed operations,
// and outputs the total time taken, in microseconds
void benchmark_ops() {
    int ops, n;

    scanf("%d, %d\n", &ops, &n);

    int* op_array = malloc(sizeof(int) * ops);
    int* i_array = malloc(sizeof(int) * ops);
    int* j_array = malloc(sizeof(int) * ops);
    for (int line = 0; line < ops; line++) {
        scanf("%d, %d, %d\n", op_array + line, i_array + line, j_array + line);
    }

    clock_t start_time = clock();

    for (int round = 0; round < 10; round ++) {

        EulerTourTree* tree = make_euler_tour_tree(n);

        for (int line = 0; line < ops; line++) {
            int op = op_array[line];
            int i = i_array[line];
            int j = j_array[line];

            if (op == 0) {
                connectivity(tree, i, j);
            } else if (op == 1) {
                link(tree, i, j);
            } else if (op == 2) {
                cut(tree, i);
            } else if (op == 3) {
                subtree_aggregate_min(tree, i);
            } else if (op == 4) {
                point_update(tree, i, j);
            } else if (op == 5) {
                subtree_update(tree, i, j);
            }
        }
    }

    clock_t end_time = clock();
    printf("%f %d %d\n", ((double) (end_time-start_time))*1e6/CLOCKS_PER_SEC/ops, ops, n);
}

int main() {
    benchmark_ops();
}