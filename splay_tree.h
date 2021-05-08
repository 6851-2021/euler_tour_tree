#include <stdint.h>

typedef uint64_t k_t;
typedef uint64_t v_t;
typedef uint64_t size_t;

typedef struct Node {
    struct Node *parent, *left, *right;
    k_t key;
    v_t value;
} Node;

Node* make_node(k_t key, v_t value);

void right_rotate(Node* node);

void left_rotate(Node* node);

void splay(Node* node);

Node* find_min(Node* node);

Node* find_max(Node* node);

Node* split_left(Node* node);

Node* split_right(Node* node);

Node* merge(Node* node1, Node* node2);

void print(Node* node);