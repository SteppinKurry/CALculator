// 10-7-24
// fml

#ifndef TREEEEEEEEEE
#define TREEEEEEEEEE

#include "calmath.h"

struct node
{

    enum {WAT, OP, NUM, VAR, MCONS} type;

    struct node* left;
    struct node* right;

    struct fraction number;
    enum operators op;
    u8 var_id;

    u64 hash;

};

struct unsimple_exp
{
    // an expression that includes numbers in symbolic notation
    // (something like sqrt(2)*sqrt(9) )

    struct node nodes[MAX_TREE_NODES];
    struct node* root;

};

struct node node_init_op(struct node* left, struct node* right, u8 operator);
struct node node_init_num(struct node* left, struct node* right, struct fraction number);
struct node node_init_var(u8 var_id);
struct node node_init_const(u8 var_id);

bool node_is_empty(struct node n);
void node_set_empty(struct node* n);
void node_copy(struct node* source, struct node* dest);

int8 node_gen_hash(struct node* n);
u64 hash_frac(struct fraction num);
u64 hash_op(u16 op);
bool subtrees_equal(struct node* a, struct node* b);
int8 node_recursive_delete(struct node* n);
u64 node_count(struct node* n);

void postorder(struct unsimple_exp* n, char* str);
void postorder_node(struct node* n, char* str, u8* str_ind);

#endif