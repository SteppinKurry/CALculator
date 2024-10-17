// 10-11-24

#include "node.h"
#include "calmath.h"
#include "ui.h"


// These are all of the expressions that this function should 
// be able to rewrite

// ADDITION
// sin^2(x) + cos^2(x) = 1

// MULTIPLICATION
// sqrt(x) * sqrt(x) = x

// POWERS

int8 sin_squared_cos_squared_rewrite(struct node* n)
{
    // 10-17-24
    // Pretty much as the title says. Give this an addition node 
    // for sin^2(x) + cos^2(x) and it'll simplify it all to 1

    // make sure there is a pow on each side
    if ( (n->left->op != POW) || (n->right->op != POW) ) { return 1; }

    // check whether or not sin/cos is the thing after 
    // the exponent
    if ( (n->left->left->op != SIN) && (n->left->left->op != COS) ) { return 1; }
    if ( (n->right->left->op != SIN) && (n->right->left->op != COS) ) { return 1; }

    // make sure there aren't two sins or two (plural for cos)
    if (n->left->left->op == n->right->left->op) { return 1; }
    
    // get the exponent/power from each of the nodes
    struct fraction left_pow = n->left->right->number;
    struct fraction right_pow = n->right->right->number;
    
    // check to see if they're both two
    if (!fractions_equal(left_pow, fraction_init(2, 1, 1))) { return 1; }
    if (!fractions_equal(right_pow, fraction_init(2, 1, 1))) { return 1; }

    // now check to see if the xs are the same
    if (!subtrees_equal(n->left->left->left, n->right->left->left)) { return 1; }

    // if we're here, then we definitely have sin^2(x) + cos^2(x)

    // delete everything after and including the initial plus sign
    node_recursive_delete(n);

    // assign 1 to the node
    *n = node_init_num(NULL, NULL, fraction_init(1, 1, 1));

    // worked out nicely
    return 0;

}


int8 addition_rewrites(struct node* n)
{
    // 10-11-24
    if (n->op != ADD) { return -1; }

    // sin^2(x) + cos^2(x) = 1
    sin_squared_cos_squared_rewrite(n);

    return 0;
}

int8 sqrt_times_sqrt_rewrite(struct node* n)
{
    // 10-17-24
    // Given a multiplication node, it will turn 
    // sqrt(x)*sqrt(x) into x and put it into the given node

    // make sure there is a sqrt on each side
    if ( (n->left->op != SQRT) || (n->right->op != SQRT) ) { return 1; }

    // make sure the xs are equal
    if (!subtrees_equal(n->left->left, n->right->left)) { return 1; }

    // at this point, we know we have sqrt(x) * sqrt(x), so set this 
    // node to be x
    node_recursive_delete(n->right);
    struct node* saved_x = n->left->left;
    node_set_empty(n->left);
    node_copy(saved_x, n);
    node_gen_hash(n);

    return 0;
}

int8 multiplication_rewrites(struct node* n)
{

    if (n->op != MUL) { return -1; }

    // sqrt(x) * sqrt(x) = x
    sqrt_times_sqrt_rewrite(n);

    return 0;
}

int8 division_rewrites(struct node* n)
{
    // nothing here yet
}

int8 unsimple_rewrite(struct node* n)
{
    // 10-11-24
    // Takes a node in a tree and tries to rewrite common 
    // mathematical expressions in a simpler form. Check the 
    // individual rewrite functions for more details on expressions 
    // that can be simplified.

    // no further simplification
    if (n->type == NUM) { return 0; }
    if (n->type == WAT) { return -1; }

    unsimple_rewrite(n->left);
    unsimple_rewrite(n->right);

    // current node must be an operator
    // if we're at a node and there're no children, 
    // that's some weird shit (probably a syntax error)
    if (n->left == NULL || n->right == NULL) { return -1; }

    // some specific rules
    addition_rewrites(n);
    multiplication_rewrites(n);
    // division_rewrites(n);


    return 0;
}