// 10-7-24

#include "node.h"


struct node node_init_num(struct node* left, struct node* right, struct fraction number)
{

    struct node a;

    a.type = NUM;
    a.left = left;
    a.right = right;
    a.number = number;
    a.op = NOOP;

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
}


void node_copy(struct node* source, struct node* dest)
{
    dest->type   =  source->type;
    dest->left   =  source->left;
    dest->right  =  source->right;
    dest->number =  source->number;
    dest->op     =  source->op;
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