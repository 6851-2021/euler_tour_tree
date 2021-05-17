#include <stdint.h>
#include <math.h>
#include "augmentations.h"

typedef int64_t k_t;    // key type
typedef int64_t v_t;    // value type
typedef uint64_t s_t;   // size type

// macros for convenience
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define INF 4e18

// storing augmentations
typedef struct Augmentations {
    #ifdef SUBTREE_MIN
    v_t min;
    #endif
    #ifdef SUBTREE_MAX
    v_t max;
    #endif
    #ifdef SUBTREE_SUM
    v_t sum;
    #endif
    #ifdef SUBTREE_SIZE
    v_t size;
    #endif
    #ifdef SUBTREE_INCREMENT
    v_t lazy;
    #endif
} Augmentations;

// splay tree node
typedef struct Node {
    struct Node *parent, *left, *right;
    k_t key;
    v_t value;
    int is_start;
    struct Augmentations augments;
} Node;

// create a splay tree node
Node* make_node(k_t key, v_t value, int is_start);

// do a range query
Augmentations query(Node* start, Node* end);

#ifdef POINT_UPDATE
// update node value
void update_node(Node* node, v_t new_value);
#endif

// update whether node is the start node
void update_node_start(Node* node, int is_start);

#ifdef SUBTREE_INCREMENT
// increment the values of the nodes in a given range
void update_range(Node* start, Node* end, v_t delta);
#endif

// bring node to root using rotations
void splay(Node* node);

// return min of tree rooted at node
// precondition: node is at root
Node* find_min(Node* node);

// return max of tree rooted at node
// precondition: node is at root
Node* find_max(Node* node);

// split tree at node, where node ends up in the right half
// returns root of left tree
Node* split_left(Node* node);

// split tree at node, where node ends up in the left half
// returns root of right tree
Node* split_right(Node* node);

// merge two trees
// precondition: node1 and node2 are roots of their respective trees
// postcondition: node1's tree is concatenated left of node2's tree
Node* merge(Node* node1, Node* node2);

// print in-order traversal of tree rooted at node
void print(Node* node);