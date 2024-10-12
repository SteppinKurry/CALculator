// 10-7-24
// You know, this shit really isn't simple
// Things relating to the unsimple_num and unsimple_exp data types

#include "node.h"
#include "parse.h"
#include "calmath.h"

struct fraction (*math_function_list[]) (struct fraction, struct fraction) = 
        {fraction_none, fraction_add, fraction_sub, fraction_mul, fraction_div, fraction_basic_power};


struct unsimple_exp unsimple_exp_init()
{
    // 10-10-24
    // makes an empty AST

    struct unsimple_exp return_this;

    // set all nodes to be "empty"
    for (int x = 0; x < MAX_TREE_NODES; x++)
    {
        //return_this.nodes[x] = node_init_op(NULL, NULL, NOOP);
        node_set_empty(&return_this.nodes[x]);
    }

    // sets the root of the tree to the address of the first node
    return_this.root = &return_this.nodes[0];
    // return_this.num_nodes = 0;

    return return_this;

}

struct node* unsimple_add_node(struct node n, struct unsimple_exp* tree)
{

    // tree is definitely full
    // if (tree->num_nodes >= MAX_TREE_NODES) { return NULL; }

    // tree may be open
    for (int x = 0; x < MAX_TREE_NODES; x++)
    {
        // found an open spot, add the node and return it's address 
        // in the tree structure
        if (node_is_empty(tree->nodes[x])) 
        {
            // copy the given node into the tree's nodes
            node_copy(&n, &tree->nodes[x]);

            // tree->num_nodes += 1;
            
            // return the address of the node in the tree
            return &tree->nodes[x];
        }

    }

    // the tree is probably full already or something else horrible happened
    return NULL;

}

int8 construct_unsimple_from_parsedstring(char parsed[MATHSTR_LEN][MAX_NUM_LEN], struct unsimple_exp* tree)
{
    // 10-10-24
    // Builds an abstract syntax tree (AST) from a postfix 
    // (parsedstring) expression

    struct node* stack[MATHSTR_LEN];
    u8 top_stack = 0;

    // loops through each element of the expression
    for (int x = 1; x < atoi(parsed[0]); x++)
    {

        // if the current element is an operator, evaluate it using the 
        // last two numbers in the stack
        if (is_operator(parsed[x][0]))
        {
            // pop last two things off stack
            struct node* b = stack[top_stack-1];
            struct node* a = stack[top_stack-2];

            top_stack -= 2;

            // build the tree
            struct node new_node = node_init_op(a, b, char_to_op(parsed[x][0]));

            // put the result at the top of the stack
            stack[top_stack] = unsimple_add_node(new_node, tree);
            top_stack += 1;

        }
        else
        {
            // if not an operator, just add to the stack
            // makes a node from the numbers in the stack

            struct node new = node_init_num(NULL, NULL, fraction_init(str_to_u64(parsed[x]), 1, 1));
            stack[top_stack] = unsimple_add_node(new, tree);

            // error
            if (stack[top_stack] == NULL) { return 1; }

            top_stack += 1;
        }

    }
    

    // it worked?
    tree->root = stack[top_stack-1];
    return 0;

}

int8 unsimple_simplify(struct unsimple_exp* tree)
{

    // rewrite/simplify parts of the expression using known 
    // rules (x*1 = x, sin^2(x)+cos^2(x) = 1, etc.)
    unsimple_rewrite(tree->root);

    return 0;
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

    if (n == NULL) { return fraction_init(0, 1, 1); }
    
    if (n->type == NUM) { return n->number; }


    if (n->type == OP) 
    { 
        struct fraction left_hand = unsimple_eval_node(n->left);
        struct fraction right_hand = unsimple_eval_node(n->right);

        return math_function_list[n->op](left_hand, right_hand);
    }


    // shouldn't happen (I think)
    return fraction_init(420, 3, 1);

}