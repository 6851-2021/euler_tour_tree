#include "euler_tour_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    while (true) {
        int n;

        printf("Enter the number of nodes for your Euler Tour Tree: ");
        scanf("%d", &n);

        EulerTourTree* tree = make_euler_tour_tree(n);

        printf("Please enter an operation in the given format:\n");
        printf("link (child) (parent)\n");
        printf("cut (node)\n");
        printf("connected (node_1) (node_2)\n");
        printf("subtree (agg_func) (node)\n");
        printf("update (node) (new_val)\n");
        printf("increment_subtree (node) (new_value)\n");

        while (true) {

            printf("> ");

            
            char op[100];

            scanf("%s", op);
            if (strcmp(op, "link") == 0) {
                long child, parent;
                scanf("%ld %ld", &child, &parent);
                link(tree, child, parent);

                // printf("success\n");
            }
            else if (strcmp(op, "cut") == 0) {
                long node;
                scanf("%ld", &node);
                cut(tree, node);

                // printf("success\n");
            }
            else if (strcmp(op, "connected") == 0) {
                long node1, node2;
                scanf("%ld %ld", &node1, &node2);
                bool is_connected = connectivity(tree, node1, node2);
                printf(is_connected ? "true\n" : "false\n");
            }
            else if (strcmp(op, "subtree") == 0) {
                char agg[100];
                long node;
                long val;

                scanf("%s %ld", agg, &node);
                printf("agg is %s\n", agg);
                if (strcmp(agg, "min") == 0) {
                    val = subtree_aggregate_min(tree, node);
                }
                else if (strcmp(agg, "max") == 0) {
                    val = subtree_aggregate_max(tree, node);
                }
                else if (strcmp(agg, "sum") == 0) {
                    val = subtree_aggregate_sum(tree, node);
                }
                else if (strcmp(agg, "size") == 0) {
                    val = subtree_aggregate_size(tree, node);
                }
                printf("%ld\n", val);
            }
            else if (strcmp(op, "update") == 0) {
                long node, new_val;
                scanf("%ld %ld", &node, &new_val);

                point_update(tree, node, new_val);
                // printf("Updated node to %ld\n", new_val);
            }
            else if (strcmp(op, "increment_subtree") == 0) {
                long node, inc_val;
                scanf("%ld %ld", &node, &inc_val);

                subtree_update(tree, node, inc_val);
                // printf("Incremented subtree values by %ld\n", inc_val);
            }
        }



    }
    

}