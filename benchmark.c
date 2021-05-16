#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void benchmark_ops(int n, int ops) {
    // FILE * fp;
    // char * line = NULL;
    // size_t len = 0;
    // ssize_t read;

    // fp = fopen("benchmark_1e3_1e5.in", "r");
    // if (fp == NULL)
    //     exit(EXIT_FAILURE);

    // while ((read = getline(&line, &len, fp)) != -1) {
    //     printf("Retrieved line of length %zu:\n", read);
    //     printf("%s", line);
    //     int op, i, j;
    //     fscanf("%d, %d, %d\n", &op, &i, &j);
    // }

    // fclose(fp);
    // if (line)
    //     free(line);
    // exit(EXIT_SUCCESS);

    // ops = 67600;
    // n = 1e3;

    scanf("%d, %d\n", &ops, &n);

    EulerTourTree* tree = make_euler_tour_tree(n);

    for (int line = 0; line < ops; line ++) {

    int op, i, j;
    scanf("%d, %d, %d\n", &op, &i, &j);

        if (op == 0) {
            connectivity(tree, i, j);
        } else if (op == 1) {
            link(tree, i, j);
            }
        
        else if (op == 2) {
            // int i = rand() % n;
            // for (int j = 0; j < n; j++) {
            //     if (has_parent[(j+i)%n]) {
                    cut(tree, i);
                    // has_parent[(j+i)%n] = false;
                    // printf("%d, %d, %d\n", op, (j+i)%n, 0);
                    // break;
                
            
        } else if (op == 3) {

            subtree_aggregate_min(tree, i);

        } else if (op == 4) {

            point_update(tree, i, j);

        } else if (op == 5) {
            subtree_update(tree, i, j);
        }

    }

    // EulerTourTree* tree = make_euler_tour_tree(n);
    // bool* has_parent = malloc(sizeof(bool) * n);
    // for (int i = 0; i < n; ++i) {
    //     has_parent[i] = false;
    // }
    // srand(32492341);
    // for (int rounds = 0; rounds < ops; ++rounds) {
    //     int op = rand() % 9;

    //     if (op > 5) {
    //         op = 1;
    //     }

    //     if (op == 0) {
    //         int i = rand() % n;
    //         int j = rand() % n;
    //         connectivity(tree, i, j);
    //         printf("%d, %d, %d\n", op, i, j);
    //     } else if (op == 1) {
    //         int i = rand() % n;
    //         for (int j = 0; j < n; j++) {
    //             if (!has_parent[(j+i)%n]) {
    //                 if ((j+i) % n == 0) continue;
    //                 k_t par = rand() % ((j+i) % n);
    //                 link(tree, (j+i)%n, par);
    //                 has_parent[(j+i) % n] = true;
    //                 printf("%d, %d, %d\n", op, (j+i)%n, par);
    //                 break;
    //             }
    //         }
    //     } else if (op == 2) {
    //         int i = rand() % n;
    //         for (int j = 0; j < n; j++) {
    //             if (has_parent[(j+i)%n]) {
    //                 cut(tree, (j+i)%n);
    //                 has_parent[(j+i)%n] = false;
    //                 printf("%d, %d, %d\n", op, (j+i)%n, 0);
    //                 break;
    //             }
    //         }
    //     } else if (op == 3) {
    //         int i = rand() % n;
    //         subtree_aggregate_min(tree, i);
    //         printf("%d, %d, %d\n", op, i, 0);
    //         // subtree_aggregate_max(tree, i);
    //         // subtree_aggregate_sum(tree, i);
    //         // subtree_aggregate_size(tree, i);
    //     } else if (op == 4) {
    //         int v = rand() % n;
    //         int val = rand() % 1000000;
    //         point_update(tree, v, val);
    //         printf("%d, %d, %d\n", op, v, val);
    //     } else if (op == 5) {
    //         int v = rand() % n;
    //         int val = rand() % 100000;
    //         subtree_update(tree, v, val);
    //         printf("%d, %d, %d\n", op, v, val);
    //     }
    // }

}

int main() {
    benchmark_ops(1e7, 1e5);
}