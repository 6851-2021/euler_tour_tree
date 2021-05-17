#include <stdint.h>
#include <math.h>
#include "augmentations.h"

typedef int64_t k_t;
typedef int64_t v_t;
typedef uint64_t s_t;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define INF 4e18

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

typedef struct Node {
    struct Node *parent, *left, *right;
    k_t key;
    v_t value;
    int is_start;
    struct Augmentations augments;
} Node;

Node* make_node(k_t key, v_t value, int is_start);

Augmentations query(Node* start, Node* end);

#ifdef POINT_UPDATE
void update_node(Node* node, v_t new_value);
#endif

void update_node_start(Node* node, int is_start);

#ifdef SUBTREE_INCREMENT
void update_range(Node* start, Node* end, v_t delta);
#endif

void splay(Node* node);

Node* find_min(Node* node);

Node* find_max(Node* node);

Node* split_left(Node* node);

Node* split_right(Node* node);

Node* merge(Node* node1, Node* node2);

void print(Node* node);