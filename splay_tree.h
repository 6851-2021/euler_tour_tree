#include <stdint.h>
#include <math.h>

typedef int64_t k_t;
typedef int64_t v_t;
typedef uint64_t size_t;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define INF 4e18

typedef struct Augmentations {
    v_t min;
    v_t max;
    v_t sum;
    v_t size;
    v_t lazy;
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

void update_node(Node* node, v_t new_value);

void update_node_start(Node* node, int is_start);

void update_range(Node* start, Node* end, v_t delta);

void splay(Node* node);

Node* find_min(Node* node);

Node* find_max(Node* node);

Node* split_left(Node* node);

Node* split_right(Node* node);

Node* merge(Node* node1, Node* node2);

void print(Node* node);