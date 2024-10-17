// 10-7-24
// fml

#ifndef TREEEEEEEEEE
#define TREEEEEEEEEE

#include "calmath.h"

struct node
{

    enum {WAT, OP, NUM} type;

    struct node* left;
    struct node* right;

    struct fraction number;
    enum operators op;

    u64 hash;

};

struct unsimple_exp
{
    // an expression that includes numbers in symbolic notation
    // (something like sqrt(2)*sqrt(2) )

    struct node nodes[MAX_TREE_NODES];
    struct node* root;
    //int8 num_nodes;

};

struct node node_init_op(struct node* left, struct node* right, u8 operator);
struct node node_init_num(struct node* left, struct node* right, struct fraction number);
bool node_is_empty(struct node n);
void node_set_empty(struct node* n);
void node_copy(struct node* source, struct node* dest);

int8 node_gen_hash(struct node* n);
u64 hash_frac(struct fraction num);
u64 hash_op(u16 op);
bool subtrees_equal(struct node* a, struct node* b);
int8 node_recursive_delete(struct node* n);

void postorder(struct unsimple_exp* n, char* str);
void postorder_node(struct node* n, char* str, u8* str_ind);

#endif