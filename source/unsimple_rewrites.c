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

int8 x_times_zero_rewrite(struct node* n)
{
    // 10-18-24
    // Given a multiplication node, it will turn 
    // x * 0 into 0 and put it into the given node

    struct fraction frac_zero = fraction_init(0, 1, 1);

    // make sure there is a zero on one of the sides
    if ( !fractions_equal(n->left->number, frac_zero) && !fractions_equal(n->right->number, frac_zero) ) { return 1; }

    // at this point, we know we have x * 0, so set this 
    // node to be 0
    node_recursive_delete(n);
    *n = node_init_num(NULL, NULL, frac_zero);

    return 0;
}

int8 multiplication_rewrites(struct node* n)
{

    if (n->op != MUL) { return -1; }

    // sqrt(x) * sqrt(x) = x
    sqrt_times_sqrt_rewrite(n);
    x_times_zero_rewrite(n);

    return 0;
}

int8 division_rewrites(struct node* n)
{
    // nothing here yet

    return 0;
}

int8 sin_rewrites(struct node* n)
{
    // 10-21-24
    // Any rewrites with just sin probably appear here
    if (n->type != OP) { return -1; }
    if (n->op != SIN) { return 1; }

    // sin(pi) = 0
    if (n->left->type == MCONS)
    {
        if (n->left->var_id == 37) // the thing on the left is pi, replace
        {
            node_recursive_delete(n);

            *n = node_init_num(NULL, NULL, fraction_init(0, 1, 1));
            return 0;
        }
    }

    // sin(pi/something)
    if (n->left->type != OP) { return 1; }
    
    // make sure there's a division
    if (n->left->op != DIV) { return 1; }
    
    // make sure pi is on top of it
    if (n->left->left->type != MCONS) { return 1; }
    if (n->left->left->var_id != 37) { return 1; }

    // definitely have pi over something at this point
    // make sure that the "something" is a number
    if (n->left->right->type == OP) { return 1; }

    // sin(pi/6) = 1/2
    if (fractions_equal(n->left->right->number, fraction_init(6, 1, 1)))
    {
        node_recursive_delete(n);
        *n = node_init_num(NULL, NULL, fraction_init(1, 2, 1));
        return 0;
    }

    // sin(pi/4) = sqrt(2)/2
    if (fractions_equal(n->left->right->number, fraction_init(4, 1, 1)))
    {
        *n = node_init_op(n->left, n->right, DIV);
        *n->left = node_init_op(n->left->left, NULL, SQRT);
        *n->left->left = node_init_num(NULL, NULL, fraction_init(2, 1, 1));
        *n->right = node_init_num(NULL, NULL, fraction_init(2, 1, 1));
        return 0;
    }

    // sin(pi/3) = sqrt(3)/2
    if (fractions_equal(n->left->right->number, fraction_init(3, 1, 1)))
    {
        *n = node_init_op(n->left, n->right, DIV);
        *n->left = node_init_op(n->left->left, NULL, SQRT);
        *n->left->left = node_init_num(NULL, NULL, fraction_init(3, 1, 1));
        *n->right = node_init_num(NULL, NULL, fraction_init(2, 1, 1));
        return 0;
    }

    // sin(pi/2) = 1
    if (fractions_equal(n->left->right->number, fraction_init(2, 1, 1)))
    {
        node_recursive_delete(n);
        *n = node_init_num(NULL, NULL, fraction_init(1, 1, 1));
        return 0;
    }

    return 1;

}

int8 cos_rewrites(struct node* n)
{
    // 10-21-24
    // Any rewrites with just cos probably appear here
    if (n->type != OP) { return -1; }
    if (n->op != COS) { return 1; }

    // cos(pi) = -1
    if (n->left->type == MCONS)
    {
        if (n->left->var_id == 37) // the thing on the left is pi, replace
        {
            node_recursive_delete(n);

            *n = node_init_num(NULL, NULL, fraction_init(1, 1, -1));
            return 0;
        }
    }

    // cos(pi/something)
    if (n->left->type != OP) { return 1; }
    
    // make sure there's a division
    if (n->left->op != DIV) { return 1; }
    
    // make sure pi is on top of it
    if (n->left->left->type != MCONS) { return 1; }
    if (n->left->left->var_id != 37) { return 1; }

    // definitely have pi over something at this point
    // make sure that the "something" is a number
    if (n->left->right->type == OP) { return 1; }

    // cos(pi/6) = sqrt(3)/2
    if (fractions_equal(n->left->right->number, fraction_init(6, 1, 1)))
    {
        *n = node_init_op(n->left, n->right, DIV);
        *n->left = node_init_op(n->left->left, NULL, SQRT);
        *n->left->left = node_init_num(NULL, NULL, fraction_init(3, 1, 1));
        *n->right = node_init_num(NULL, NULL, fraction_init(2, 1, 1));
        return 0;
    }

    // cos(pi/4) = sqrt(2)/2
    if (fractions_equal(n->left->right->number, fraction_init(4, 1, 1)))
    {
        *n = node_init_op(n->left, n->right, DIV);
        *n->left = node_init_op(n->left->left, NULL, SQRT);
        *n->left->left = node_init_num(NULL, NULL, fraction_init(2, 1, 1));
        *n->right = node_init_num(NULL, NULL, fraction_init(2, 1, 1));
        return 0;
    }

    // cos(pi/3) = 1/2
    if (fractions_equal(n->left->right->number, fraction_init(3, 1, 1)))
    {
        node_recursive_delete(n);
        *n = node_init_num(NULL, NULL, fraction_init(1, 2, 1));
        return 0;
    }

    // cos(pi/2) = 0
    if (fractions_equal(n->left->right->number, fraction_init(2, 1, 1)))
    {
        node_recursive_delete(n);
        *n = node_init_num(NULL, NULL, fraction_init(0, 1, 1));
        return 0;
    }

    return 1;

}

int8 trig_rewrites(struct node* n)
{
    // rewrites related to trigonometry functions
    sin_rewrites(n);
    cos_rewrites(n);


    return 0;

}

int8 unsimple_rewrite(struct node* n)
{
    // 10-11-24
    // Takes a node in a tree and tries to rewrite common 
    // mathematical expressions in a simpler form. Check the 
    // individual rewrite functions for more details on expressions 
    // that can be simplified.

    // no further simplification
    if (n == NULL) { return -1; }
    if (n->type == NUM) { return 0; }
    if (n->type == WAT) { return -1; }
    if (n->type == VAR) { return 0; }
    if (n->type == MCONS) { return 0; }    

    unsimple_rewrite(n->left);
    unsimple_rewrite(n->right);

    // current node must be an operator
    // if we're at a node and there're no children, 
    // that's some weird shit (probably a syntax error)
    if (n->left == NULL && n->right == NULL) { return -1; }


    // some specific rules
    addition_rewrites(n);
    multiplication_rewrites(n);
    division_rewrites(n);
    trig_rewrites(n);


    return 0;
}