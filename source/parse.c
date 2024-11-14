// 3-18-24

#include "parse.h"

int num_ops = 12;
char valid_ops[][5] = { "+", "-", "*", "/", "(", ")", "^", 
	                    "sin", "cos", "E", "tan", "sqrt"};

enum operators str_to_op(char* c)
{
    if (!strcmp(c, "+")) return ADD;
    if (!strcmp(c, "-")) return SUB;
    if (!strcmp(c, "*")) return MUL;
    if (!strcmp(c, "/")) return DIV;
    if (!strcmp(c, "^")) return POW;
    if (!strcmp(c, "(")) return LPAREN;
    if (!strcmp(c, ")")) return RPAREN;
    if (!strcmp(c, "sin")) return SIN;
    if (!strcmp(c, "cos")) return COS;
    if (!strcmp(c, "tan")) return TAN;
    if (!strcmp(c, "E")) return E;
    if (!strcmp(c, "sqrt")) return SQRT;

    return NOOP;
}

bool is_user_var(char* str)
{
    // 10-21-24
    // Returns true if the given string is a user-defined variable 
    // (A, B, etc.) or if the given string is a constant that could 
    // be treated like a variable (pi, e, etc.)
    u8 ord = user_var_to_int(str);

    if (ord > 0 && ord < 26) { return true; }

    if (!strcmp(str, "ans")) { return true; }

    if (!strcmp(str, "x")) { return true; }

    return false;
}

bool is_math_const(char* str)
{
    char math_consts[][4] = {"pi", "e"};
    u8 num_consts = 2;

    for (int x = 0; x < num_consts; x++)
    {
        if (!strcmp(str, math_consts[x])) { return true; }
    }

    return false;
}

u8 user_var_to_int(char* str)
{
    // 10-21-24
    // Returns an integer representing the given user-variable string

    if (!strcmp(str, "ans")) { return 27; }
    if (!strcmp(str, "pi")) { return 37; }
    if (!strcmp(str, "e")) { return 38; }

    return str[0] - 64;
}

bool is_math_func(enum operators op)
{

    // 10-12-24
    // Returns true if the given operator is a math function/
    // just a unary operator

    if (op == ADD || op == SUB || op == DIV || op == MUL || op == POW ||
        op == LPAREN || op == RPAREN || op == E)

        {
            return false;
        }

    return true;


}

bool is_operator(char* e)
{
    // 10-12-24
    // Tests whether or not the given string is a 
    // recognized operator

    for (int x = 0; x < num_ops; x ++)
    {
        if (!strcmp(e, valid_ops[x])) { return true; }
    }

    return false;
}

u8 get_op_precedence(char* a)
{
    // 10-12-24
    // This is, admittedly, another smooth-brain implementation

    // return 0 if it's not even an operator
    if (!is_operator(a)) { return 0; }

    // basic operators
    if (!strcmp(a, "+") || !strcmp(a, "-")) { return 1; }
    if (!strcmp(a, "*") || !strcmp(a, "/")) { return 2; }
    if (!strcmp(a, "^")) { return 3; }
    if (!strcmp(a, "(") || !strcmp(a, ")")) { return 255; } // ultimate precedence
    

    // anything unspecified should just be a math function, and therefore 
    // a higher precedence than anything else 
    return 4;
}

bool a_has_precedence(char* a, char* b)
{
    u8 a_prec = get_op_precedence(a);
    u8 b_prec = get_op_precedence(b);

    return a_prec >= b_prec;
}


u8 tokenize(char* expression, char mathstring[MATHSTR_LEN][MAX_NUM_LEN])
{
    // 9-9-24 (updated 10-12-24 to add multi-character operator support)
    // Accepts a string with a mathematical expression and breaks 
    // it into smaller pieces for the parser. Outputs a 2D array 
    // with each individual part of the given expression. The expression 
    // is split up based upon operators

    // setup a few things
    char current_num[MAX_NUM_LEN+3];
    char current_op[MAX_NUM_LEN+3];

    bool making_op = false;
    u8 output_index = 1;
    u8 current_num_len = 0;
    u8 current_op_len = 0;

    u8 expression_len = strlen(expression);

    // loop through the given expression
    for (int x = 0; x < expression_len; x++)
    {
        
        if (!making_op && !isdigit((unsigned char)expression[x]))
        {
            making_op = true;
        }

        if (making_op)
        {
            // only add the character if current has room left
            if (current_op_len < MAX_NUM_LEN)
            {
                current_op[current_op_len] = expression[x];
                current_op[current_op_len+1] = '\0';
                current_op_len += 1;
            }
            
        }
        else
        {
            // only add the character if current has room left
            if (current_num_len < MAX_NUM_LEN)
            {
                current_num[current_num_len] = expression[x];
                current_num[current_num_len+1] = '\0';
                current_num_len += 1;
            }
            
            if (current_num_len >= MAX_NUM_LEN)
            {
                current_num[MAX_NUM_LEN-1] = '\0';
            }
        }

        if (making_op && (is_operator(current_op) || is_user_var(current_op) || is_math_const(current_op)) )
        {

            // copy over the current number and the current operator

            if (current_num_len == 0) // two operators in a row (parentheses or something)
            {
                snprintf(mathstring[output_index], MAX_NUM_LEN, "%.*s", current_op_len, current_op);
                output_index -= 1;
            }
            else    // normal behavior
            {

                // make sure the number isn't too big
                if (current_num_len > MAX_NUM_LEN) { current_num_len = MAX_NUM_LEN; }

                // copy everything over
                snprintf(mathstring[output_index], MAX_NUM_LEN, "%.*s", current_num_len, current_num);
                snprintf(mathstring[output_index+1], MAX_NUM_LEN, "%.*s", current_op_len, current_op);
            }

            // reset some things
            output_index += 2;
            current_num_len = 0;
            current_op_len = 0;

            making_op = false;

        }

    }

    // append the final current to the output
    if (making_op)
    {
        if (current_op_len >= MAX_NUM_LEN) { current_op_len = MAX_NUM_LEN; }
        snprintf(mathstring[output_index], MAX_NUM_LEN, "%.*s", current_op_len, current_op);

    }
    
    else if (current_num_len != 0)
    {
        if (current_num_len >= MAX_NUM_LEN) { current_num_len = MAX_NUM_LEN; }
        snprintf(mathstring[output_index], MAX_NUM_LEN, "%.*s", current_num_len, current_num);

        output_index += 1;

    }

    // store the number of elements in the output in the output
    sprintf(mathstring[0], "%d", output_index-1);

    // it worked (hopefully)
    return 0;
    

}

bool is_valid_number(char* number)
{
    // 9-9-24
    // Checks whether or not the given string is a number

    u8 len = strlen(number);
    
    // return if empty
    if (len <= 0) { return false; }

    // check everything else
    for (int x = 0; x < len; x++)
    {
        if (!isdigit((unsigned char)number[x])) { return false; }
    }

    // if here, it's a number
    return true;

}

u8 parse(char mathstring[MATHSTR_LEN][MAX_NUM_LEN], char parsed[MATHSTR_LEN][MAX_NUM_LEN])
{

    // 9-9-24 (modified 10-12-24 for the multi-character operator support)
    // Uses the shunting yard algorithm to turn an infix math 
    // expression (mathstring) into the same expression represented 
    // in postfix notation. You can get a mathstring from the tokenize 
    // function.

    char op_stack[MATHSTR_LEN][5];
    u8 parsed_ind = 1;
    u8 op_stack_top = 0;
    
    // loops through each thing in the expression
    for (int x = 1; x < atoi(mathstring[0]) + 1; x++)
    {

        // if it's a number, user-defined variable, or mathematical constant add it to the output
        if (is_valid_number(mathstring[x]) || is_user_var(mathstring[x]) || is_math_const(mathstring[x])) 
        { 
            strcpy(parsed[parsed_ind], mathstring[x]); 
            parsed_ind += 1;

        }
        // if it's an operator and not a parenthesis
        else if ( is_operator(mathstring[x]) && strcmp(mathstring[x], "(") && strcmp(mathstring[x], ")") )
        {

            // while the thing at the top of the op stack has greater 
            // precedence than the current op
            while ( (op_stack_top != 0) && (a_has_precedence(op_stack[op_stack_top], mathstring[x])) 
                     && (strcmp(op_stack[op_stack_top], "(")) )
            {

                // remove things from the operator stack and put 
                // them in the output
                strcpy(parsed[parsed_ind], op_stack[op_stack_top]);

                op_stack_top -= 1;
                parsed_ind += 1;

            }

            // add the current operator to the stack
            op_stack_top += 1;
            strcpy(op_stack[op_stack_top], mathstring[x]);
                            
        }

        // if open parenthesis
        else if (!strcmp(mathstring[x], "(") )
        {
            // add it to the stack like usual
            op_stack_top += 1;
            strcpy(op_stack[op_stack_top], mathstring[x]);
        }

        // if close parenthesis
        else if (!strcmp(mathstring[x], ")"))
        {

            // remove everything from the op stack until you 
            // reach an open parenthesis    
            while (strcmp(op_stack[op_stack_top], "("))
            {
                strcpy(parsed[parsed_ind], op_stack[op_stack_top]);

                op_stack_top -= 1;
                parsed_ind += 1;
            }

            // if the current thing at the top of the stack isn't an 
            // open parenthesis, complain
            if (strcmp(op_stack[op_stack_top], "("))
            {
                char weenres[80];
                u8 peen = 15;
                sprintf(weenres, "s000000000000 angry: %s", op_stack[op_stack_top]);
                calc_main_print(weenres, &peen, 0);

                // return with error 1
                return 1;
            }
            op_stack_top -= 1;
        }

    }

    // move all of the remaining operators off the stack and 
    // into the output
    while (op_stack_top > 0)
    {
        strcpy(parsed[parsed_ind], op_stack[op_stack_top]);

        parsed_ind += 1;
        op_stack_top -= 1;
    }

    // put the final number of elements in the expression 
    // into the beginning of it
    sprintf(parsed[0], "%d", parsed_ind);

    // worked without error
    return 0;
}