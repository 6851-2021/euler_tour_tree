#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test_gen() {
    int ops, n;
    scanf("%d %d", &ops, &n);
    printf("%d, %d\n", ops, n);

    EulerTourTree* tree = make_euler_tour_tree(n);

    bool* has_parent = malloc(sizeof(bool) * n);
    for (int i = 0; i < n; ++i) {
        has_parent[i] = false;
    }
    srand(32492341);

    for (int rounds = 0; rounds < ops;) {
        int op = rand() % 9;

        if (op > 5) {
            op = 1;
        }

        if (op == 0) {
            int i = rand() % n;
            int j = rand() % n;
            connectivity(tree, i, j);
            printf("%d, %d, %d\n", op, i, j);
            rounds++;
        } else if (op == 1) {
            int i = rand() % n;
            for (int j = 0; j < n; j++) {
                if (!has_parent[(j+i)%n]) {
                    if ((j+i) % n == 0) continue;
                    k_t par = rand() % ((j+i) % n);
                    link(tree, (j+i)%n, par);
                    has_parent[(j+i) % n] = true;
                    printf("%d, %d, %ld\n", op, (j+i)%n, par);
                    rounds++;
                    break;
                }
            }
        } else if (op == 2) {
            int i = rand() % n;
            for (int j = 0; j < n; j++) {
                if (has_parent[(j+i)%n]) {
                    cut(tree, (j+i)%n);
                    has_parent[(j+i)%n] = false;
                    printf("%d, %d, %d\n", op, (j+i)%n, 0);
                    rounds++;
                    break;
                }
            }
        } else if (op == 3) {
            int i = rand() % n;
            subtree_aggregate_min(tree, i);
            printf("%d, %d, %d\n", op, i, 0);
            rounds++;
            // subtree_aggregate_max(tree, i);
            // subtree_aggregate_sum(tree, i);
            // subtree_aggregate_size(tree, i);
        } else if (op == 4) {
            int v = rand() % n;
            int val = rand() % 1000000;
            point_update(tree, v, val);
            printf("%d, %d, %d\n", op, v, val);
            rounds++;
        } else if (op == 5) {
            int v = rand() % n;
            int val = rand() % 100000;
            subtree_update(tree, v, val);
            printf("%d, %d, %d\n", op, v, val);
            rounds++;
        }
    }
}

int main() {
    test_gen();
}