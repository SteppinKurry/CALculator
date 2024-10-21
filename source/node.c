// 10-7-24

#include <stdio.h>
#include "node.h"
#include "ui.h"
#include "MurmurHash3.h"


struct node node_init_num(struct node* left, struct node* right, struct fraction number)
{

    struct node a;

    a.type = NUM;
    a.left = left;
    a.right = right;
    a.number = number;
    a.op = NOOP;

    a.hash = hash_frac(number);

    return a;

}

struct node node_init_op(struct node* left, struct node* right, u8 op)
{

    struct node a;

    a.type = OP;
    a.left = left;
    a.right = right;
    a.number = fraction_init(0, 1, 1);
    a.op = op;

    a.hash = 0;

    return a;

}

bool node_is_empty(struct node n)
{

    // a node is empty if it is an operator type and 
    // has an operator of no op (noop)

    // if it's a number, it isn't empty
    if (n.type == NUM) { return false; }

    // if it isn't a noop, it isn't empty
    if (n.op != NOOP) { return false; }

    // if it has a hash value, it isn't empty ig
    if (n.hash != 0) { return false; }


    // otherwise, it's empty
    return true;
}

void node_set_empty(struct node* n)
{
    // 10-10-24
    // A node is considered empty if it has an operator type 
    // but has no operation (noop)
    n->type = OP;
    n->op = NOOP;
    n->hash = 0;
}


void node_copy(struct node* source, struct node* dest)
{
    dest->type   =  source->type;
    dest->left   =  source->left;
    dest->right  =  source->right;
    dest->number =  source->number;
    dest->op     =  source->op;
}

u64 hash_frac(struct fraction num)
{
    
    // ensure the input and outputs are large enough
    char ween[(MAX_NUM_LEN*2)+7];
    uint32_t peen;

    fraction_simplify(num);

    sprintf(ween, "%lld/%lld", num.numerator, num.denominator);
    MurmurHash3_x86_32(ween, strlen(ween), HASH_SEED, &peen);

    return peen;

}

u64 hash_op(u16 op)
{

    char ween[] = {op, op, op, '\0'};

    uint32_t peen;

    MurmurHash3_x86_32(ween, strlen(ween), HASH_SEED, &peen);

    return peen;

}

int8 node_gen_hash(struct node* n)
{
    // 10-17-24
    // Generates a unique hash for a given node. The hash is 
    // calculated based upon the given node, but also the nodes to 
    // its left and right. If two nodes' hashes are equal, then 
    // they represent identical expressions

    if (n == NULL || n->type == WAT) { return -1; }

    if (n->type == NUM) 
    { 
        n->hash = hash_frac(n->number);
        return 0;
    }

    // the given node must be an operator if we made it here
    int8 left = node_gen_hash(n->left);
    int8 right = node_gen_hash(n->right);

    u64 left_hash = 0;
    u64 right_hash = 0;

    // zero means no problems
    if (left == 0) { left_hash = n->left->hash; }
    if (right == 0) { right_hash = n->right->hash; }

    // generate and store the hash for the node
    n->hash = hash_op(n->op) ^ (left_hash * 31) ^ (right_hash * 37);

    return 0;

}

bool subtrees_equal(struct node* a, struct node* b)
{
    return a->hash == b->hash;
}

int8 node_recursive_delete(struct node* n)
{
    
    if (n == NULL) { return 1; }

    node_recursive_delete(n->left);
    node_recursive_delete(n->right);

    node_set_empty(n);

    return 0;

}

void postorder(struct unsimple_exp* tree, char* str)
{

    u8 ween =  0;
    postorder_node(tree->root, str, &ween);

    str[ween] = '\0';

}

void postorder_node(struct node* n, char* str, u8* str_ind)
{
    if (n->left != NULL)
    {
        postorder_node(n->left, str, str_ind);
    }
    if (n->right != NULL)
    {
        postorder_node(n->right, str, str_ind);
    }

    if (n->type == OP) { str[*str_ind] = n->op + '0'; }
    if (n->type == NUM) { str[*str_ind] = n->number.numerator + '0'; } 

    *str_ind += 1;
}