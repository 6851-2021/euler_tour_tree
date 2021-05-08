#include <stdint.h>

typedef uint64_t k_t;
typedef uint64_t v_t;

typedef struct Node {
    struct Node *parent, *left, *right;
    k_t key;
    v_t value;
} Node;

Node* make_node(k_t key, v_t value);

void right_rotate(Node* node);

void left_rotate(Node* node);

void splay(Node* node);

void print(Node* node);