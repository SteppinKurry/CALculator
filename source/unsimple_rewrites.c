// 10-11-24

#include "node.h"
#include "calmath.h"
#include "ui.h"


// These are all of the expressions that this function should 
// be able to rewrite

// ADDITION
// x + 0 = x
// sin^2(x) + cos^2(x) = 1

// MULTIPLICATION
// x * 1 = x
// x * 0 = 0

// POWERS
// x ^ 1 = x
// x ^ 1


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
    // if we're at a node and there's no children, 
    // that's some weird shit (probably a syntax error)
    if (n->left == NULL || n->right == NULL) { return -1; }

    // some specific rules
    multiplication_rewrites(n);
    addition_rewrites(n);
    division_rewrites(n);


    return 0;
}

int8 addition_rewrites(struct node* n)
{
    // 10-11-24
    

    if (n->op == ADD) { return -1; }

    // sin^2(x) + cos^2(x) = 1
    // if ( (n->left->op == POW) && (n->right->op == POW) )
    // {
    //     // check whether or not sin/cos is the thing after 
    //     // the exponent
    //     if (n->left->left->op != SIN || n->left->left->op != COS) { break; }
    //     if (n->right->left->op != SIN || n->left->right->op != COS) { break; }

    //     // make sure they're not the same function
    //     if (n->left->op == n->right->op) { break; }
        
    //     // get the exponent/power from each of the nodes
    //     struct fraction left_pow = n->left->right;
    //     struct fraction right_pow = n->right->right;
        
    //     // check to see if they're both two
    //     if (!fraction_equals(left_pow, fraction_init(2, 1, 1))) { break; }
    //     if (!fraction_equals(right_pow, fraction_init(2, 1, 1))) { break; }

    // }

    return 0;

}

int8 multiplication_rewrites(struct node* n)
{

    if (n->op != MUL) { return -1; }

    // make sure one of the operands is a number
    if (n->left->type == NUM || n->right->type == NUM)
    {

        // x * 1 = x ------------------------------------------------------------

        // check whether or not each operand is 1
        struct fraction frac_one = fraction_init(1, 1, 1);
        bool left_num_is_one = fractions_equal(frac_one, n->left->number);
        bool right_num_is_one = fractions_equal(frac_one, n->right->number);

        // if one of them is 1, replace the current node with the 
        // node that isn't 1 (technically, the other node could be 1 as well...)
        if (left_num_is_one)
        {
            // reset (delete) the left node
            node_set_empty(n->left);

            // save and move the right one's contents into the current
            struct node* old_right = n->right;
            node_copy(n->right, n);

            // delete the old right one
            node_set_empty(old_right);

        }
        else if (right_num_is_one)
        {
            node_copy(n->left, n);
        }


        // x * 0 = 0 ------------------------------------------------------------

        // check whether or not each operand is 0
        struct fraction frac_zero = fraction_init(0, 1, 1);
        bool left_num_is_zero = fractions_equal(frac_zero, n->left->number);
        bool right_num_is_zero = fractions_equal(frac_zero, n->right->number);

        // if either operand is zero, delete all subchildren and set this to zero
        if (left_num_is_zero || right_num_is_zero)
        {   
            // reset (delete) left and right nodes
            node_set_empty(n->left);
            node_set_empty(n->right);

            // set current node to zero
            *n = node_init_num(NULL, NULL, fraction_init(0, 1, 1));

        }


    }

    return 0;
}

int8 division_rewrites(struct node* n)
{
    // simplify tree by making fractions where applicable
    if (n->op != DIV) { return -1; }
    
    // save values from left and right node
    struct fraction a = n->left->number;
    struct fraction b = n->right->number;

    // reset (delete) left and right nodes
    node_set_empty(n->left);
    node_set_empty(n->right);

    // change current node to be a fraction representing 
    // a division between a and b
    a = fraction_div(a, b);

    *n = node_init_num(NULL, NULL, a);

    return 0;

}