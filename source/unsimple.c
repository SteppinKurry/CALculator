// 10-7-24
// You know, this shit really isn't simple
// Things relating to the unsimple_num and unsimple_exp data types

#include "node.h"
#include "parse.h"
#include "calmath.h"

struct fraction (*math_function_list[]) (struct fraction, struct fraction) = 
        {fraction_none, fraction_add, fraction_sub, fraction_mul, fraction_div, fraction_basic_power, fraction_none, 
         fraction_none, fraction_sin, fraction_cos, fraction_tan, fraction_none, fraction_none, psqrt};

bool is_valid_address(struct node* n, struct unsimple_exp* tree)
{

    for (int x = 0; x < MAX_TREE_NODES; x++)
    {
        if (&tree->nodes[x] == n) return true;
    }

    return false;
}

struct unsimple_exp unsimple_exp_init()
{
    // 10-10-24
    // makes an empty AST

    struct unsimple_exp return_this;


    // set all other nodes to be "empty"
    for (int x = 0; x < MAX_TREE_NODES; x++)
    {
        node_set_empty(&return_this.nodes[x]);
    }

    // sets the root of the tree to the address of the first node
    return_this.root = &return_this.nodes[0];

    return return_this;

}

struct node* unsimple_add_node(struct node n, struct unsimple_exp* tree)
{
    // 10-10-24
    // Adds a new node to the first empty spot in the tree's node array. Does 
    // not necessarily link the new node to any other nodes, so make sure you 
    // do that manually before/after adding it to the tree.

    // find a place in the tree to put the node
    for (int x = 0; x < MAX_TREE_NODES; x++)
    {
        // found an open spot, add the node and return its address 
        // in the tree structure
        if (node_is_empty(tree->nodes[x])) 
        {
            // copy the given node into the tree's nodes
            node_copy(&n, &tree->nodes[x]);

            // return the address of the node in the tree
            return &tree->nodes[x];
        }

    }

    // the tree is probably full already or something else horrible happened
    annoyed_print("FUCKING TREE OVERFLOW");
    return NULL;
}

struct node* unsimple_copy_node(struct node* source_node, struct unsimple_exp* dest)
{
    
    // if the source node is empty, then we're at the end of the 
    // source tree. We don't need to check dest
    if (source_node == NULL) { return NULL; }

    // the left and right nodes are built before this one, so we store 
    // the (correct) addresses of the left and right nodes
    struct node* left = unsimple_copy_node(source_node->left, dest);
    struct node* right = unsimple_copy_node(source_node->right, dest);

    // copy source node into new node (wrong addresses included)
    struct node* new = unsimple_add_node(*source_node, dest);

    // update the left and right to point to the nodes in the destination 
    // tree instead of the nodes in the source tree (the current node points
    // to nodes in the source tree because of the unsimple_add_node(); obvi no good)

    if (new == NULL) { annoyed_print("definitely panic"); }
    else
    {
        new->left = left;
        new->right = right;
    }

    node_gen_hash(new);

    if (new->hash != source_node->hash) 
    { 
        char ween[90];
        sprintf(ween, "n: %llu s: %llu", new->hash, source_node->hash);
        //annoyed_print(ween);
    }

    // return the address of this node
    return new;

}

int8 unsimple_copy(struct unsimple_exp* source, struct unsimple_exp* dest)
{

    // reset the destination tree
    *dest = unsimple_exp_init();

    // recursively builds the destination tree based upon the 
    // source tree
    dest->root = unsimple_copy_node(source->root, dest);

    if (dest->root == NULL) { annoyed_print("WHAT"); return -69; }

    // generate a hash for the destination tree
    node_gen_hash(dest->root);

    // make sure the two trees are actually equal
    if (subtrees_equal(source->root, dest->root)) { return 0; }

    // return error if not
    return -1;

}


int8 merge_trees(struct node* dest_node, struct unsimple_exp* dest_tree, struct node* source_node)
{
    // dest_node is the address of the node in the tree that you want 
    // to start merging it. If you're substituting for a variable, dest_node 
    // should be the address of that variable node.

    // dest_tree is the tree that dest_node belongs to.

    // source_node is the node in the tree you want to start copying from. I imagine 
    // this will usually be the root of the tree you want to copy into the 
    // destination tree.

    // removes everything after and including the node we're copying
    // everything to
    node_recursive_delete(dest_node->left);
    node_recursive_delete(dest_node->right);

    // recursively travels the source tree and adds each node to the destination 
    // tree's nodes (and correctly assigns addresses (hopefully) )
    struct node* new_current_node = unsimple_copy_node(source_node, dest_tree);

    // copy the subtree that was built in the helper into the destination node
    node_copy(new_current_node, dest_node);

    // generate a new hash for the destinatoon tree
    node_gen_hash(dest_tree->root);

    return 0;
}

int8 unsimple_sub_vars_node(struct node* n, struct unsimple_exp* tree, struct unsimple_exp* values, bool graphing)
{
    // 10-21-24
    // Substitutes values into variables in expressions

    // return if the given node is NULL or the wrong type
    if (n == NULL) { return -1; }
    if (n->type == WAT) { return -1; }
    if (n->type == NUM) { return 1; }

    // could have either an op node or a var node
    unsimple_sub_vars_node(n->left, tree, values, graphing);
    unsimple_sub_vars_node(n->right, tree, values, graphing);

    if (n->type != VAR) { return 1; }

    // we must have a var node if we made it here, so change it 
    // to a num node and substitute based upon the given array 
    // of variable values

    // the variable x (id 120-64) is exclusively for graphing, so check 
    // whether or not graphing is happening
    if (n->var_id == 120-64)
    {
        // if not graphing, leave it alone
        if (!graphing) { return 4; }

        // otherwise, replace the x with an A (because the graphing function assumes
        // that A is the variable)
        n->var_id = 1;

    }
    merge_trees(n, tree, values[n->var_id].root);
    return 0;
}

int8 unsimple_sub_vars(struct unsimple_exp* tree, struct unsimple_exp* values, bool graphing)
{

    return unsimple_sub_vars_node(tree->root, tree, values, graphing);

}

int8 unsimple_contains_node(struct node* n, struct node* tree_node)
{

    // if the node points to null, it's not in the tree
    if (n == NULL) { return 1; }

    // if the tree node's address is null, we've gone too far
    if (tree_node == NULL) { return false; }

    // check left and right nodes
    bool left = unsimple_contains_node(n, tree_node->left);
    bool right = unsimple_contains_node(n, tree_node->right);

    // return true if the right or left had a match
    if (left == 0 || right == 0) { return 0; }

    // if n matches the current node, we found a match
    if (n == tree_node) { return 0; }

    return -1;

}

bool unsimple_is_valid_node(struct node* n)
{
    return false;
}

int8 unsimple_is_valid(struct unsimple_exp* tree)
{

    // make sure that none of the nodes have references to 
    // nodes outside of the tree

    for (int x = 0; x < MAX_TREE_NODES; x++)
    {
        // checks that both left and right point to either 
        // null or other elements in the tree

        int8 left_in_tree = unsimple_contains_node(tree->nodes[x].left, tree->root);
        int8 right_in_tree = unsimple_contains_node(tree->nodes[x].right, tree->root);

        if (left_in_tree == -1) { return 1; }
        if (right_in_tree == -1) { return 2; }

    }


    return 0;

}

int8 construct_unsimple_from_parsedstring(char parsed[MATHSTR_LEN][MAX_NUM_LEN], struct unsimple_exp* tree)
{
    // 10-10-24 (updated 10-12-24 for the multi-character operators)
    // Builds an abstract syntax tree (AST) from a postfix 
    // (parsedstring) expression

    node_recursive_delete(tree->root);

    struct node* stack[MATHSTR_LEN];
    u8 top_stack = 0;

    // loops through each element of the expression
    for (int x = 1; x < atoi(parsed[0]); x++)
    {

        // if the current element is an operator, evaluate it using the 
        // last two numbers in the stack
        if (is_operator(parsed[x]))
        {
            struct node new_node;

            if (is_math_func(str_to_op(parsed[x])))
            {
                // if it's a unary operator, we only expect one operand
                struct node* a = stack[top_stack-1];
                top_stack -= 1;

                // build tree
                new_node = node_init_op(a, NULL, str_to_op(parsed[x]));

            }
            else
            {

                // pop last two things off stack
                struct node* b = stack[top_stack-1];
                struct node* a = stack[top_stack-2];

                top_stack -= 2;

                // build the tree
                new_node = node_init_op(a, b, str_to_op(parsed[x]));

            }

            // put the result at the top of the stack
            stack[top_stack] = unsimple_add_node(new_node, tree);
            top_stack += 1;

        }
        else
        {
            // if not an operator, just add to the stack
            struct node new;
            
            if (is_user_var(parsed[x]))
            {
                // makes a node from a user-defined variable
                new = node_init_var(user_var_to_int(parsed[x]));
            }
            else if (is_math_const(parsed[x]))
            {
                // makes a node from a constant (pi, e, etc.)
                new = node_init_const(user_var_to_int(parsed[x]));
            }
            else
            {
                // makes a node from the given numbers
                new = node_init_num(NULL, NULL, fraction_init(str_to_u64(parsed[x]), 1, 1));
            }

            stack[top_stack] = unsimple_add_node(new, tree);

            // error
            if (stack[top_stack] == NULL) { return 1; }

            top_stack += 1;
        }

    }
    
                
    // it worked?

    tree->root = stack[top_stack-1];
    node_gen_hash(tree->root);

    return 0;

}

int8 unsimple_simplify(struct unsimple_exp* tree)
{
    // This function simplifies a mathematical expression 
    // as much as possible. In cases where the answer is a 
    // rational number, this usually is as close as you need 
    // to get to the answer. For other cases, this will do 
    // its damndest to get as close as possible to a simplified 
    // representation. 
    
    // does any operations that it knows will result in a rational 
    // number
    unsimple_combine_scalars_node(tree->root);

    // rewrite/simplify parts of the expression using known 
    // rules ( sin^2(x)+cos^2(x) = 1, etc.)
    unsimple_rewrite(tree->root);

    unsimple_combine_scalars_node(tree->root);

    return 0;
}

int8 unsimple_combine_scalars_node(struct node* n)
{
    // 10-17-24
    // Combines the easily-combinable numbers in a tree. For example, if 
    // the expression was something like sin(3+9), it would be sin(12) after 
    // this function executes. This is a preliminary step in simplifying 
    // everything.

    // node doesn't exist, so return error
    if (n == NULL || n->type == WAT) { return -1; }

    // if it's already a number, return 0
    if (n->type == NUM) { return 0; }

    // if we're here, n must be an operator

    // do subtrees first
    int8 left = unsimple_combine_scalars_node(n->left);
    int8 right = unsimple_combine_scalars_node(n->right);

    // none of these result in irrational numbers, so the numbers can 
    // just be combined
    if (n->op == ADD || n->op == SUB || n->op == MUL || n->op == DIV || n->op == POW)
    {
        // if one of the sides can't be simplified any further, do nothing
        // 0 means good to keep combining

        if (left != 0 || right != 0)
        {
            return -1;
        }

        // powers are allowed if the denominator of the power is one
        if (n->right->number.denominator != 1) { return -1; }

        // otherwise, just combine them and replace this node with the new value

        // calculate the new number
        struct fraction combined_frac = math_function_list[n->op](n->left->number, n->right->number);

        // reset (delete) left and right nodes
        node_set_empty(n->left);
        node_set_empty(n->right);

        // turn this node into a number node and set its value to the 
        // previously calculated value
        *n = node_init_num(NULL, NULL, combined_frac);
        node_gen_hash(n);

        // return 0 so that we can keep going later
        return 0;
    }

    // if we're here, n is some weird, potentially irrational operator, and
    // we can't easily combine that, so do nothing with it
    return -1;


}

struct fraction unsimple_evaluate(struct unsimple_exp* tree)
{
    // 10-10-24
    // Kind of a helper function. This one simplifies a potentially 
    // unsimplified fraction ig

    return fraction_simplify(unsimple_eval_node(tree->root));

}

struct fraction unsimple_eval_node(struct node* n)
{
    // 10-10-24
    // Traverses an AST and evaluates all of it based upon 
    // the operators and operands. Returns an unsimplified 
    // fraction

    struct fraction mcons_list[40];
    mcons_list[37] = fraction_init(314159265, 100000000, 1);
    mcons_list[38] = fraction_init(271828182, 100000000, 1);

    if (n == NULL) { return fraction_init(0, 1, 1); }
    
    if (n->type == NUM) { return n->number; }

    if (n->type == MCONS) { return mcons_list[n->var_id]; }
    
    // if we found an uninitialized user-variable, just give it 17
    if (n->type == VAR) { return fraction_init(17, 1, 1); }

    if (n->type == OP) 
    { 
        struct fraction left_hand = unsimple_eval_node(n->left);
        struct fraction right_hand = unsimple_eval_node(n->right);
        
        // if either the left or right is an error fraction (div by zero, 
        // sqrt(-1), etc), then just return an error fraction
        
        if (fraction_is_error(left_hand) || fraction_is_error(right_hand))
        {
            return fraction_init_error();
        }

        return math_function_list[n->op](left_hand, right_hand);
    }


    // shouldn't happen (I think)
    return fraction_init(420, 3, 1);

}