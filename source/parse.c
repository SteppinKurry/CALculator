// 3-18-24

#include "parse.h"

char operatorsOld[] = { '+', '1', '-', '1', '*', '2', '/', '2', '^', '3', '(', '4', ')', '4' };
int num_ops = 7;

enum operators char_to_op(char c)
{
    if (c == '+') return ADD;
    if (c == '-') return SUB;
    if (c == '*') return MUL;
    if (c == '/') return DIV;
    if (c == '^') return POW;
    if (c == '(') return LPAREN;
    if (c == ')') return RPAREN;

    return NOOP;
}

char is_operator(char e)
{
    // 9-7-24
    // Tests whether or not the given character is a 
    // recognized operator

    for (int x = 0; x < num_ops * 2; x += 2)
    {
        if (e == operatorsOld[x]) { return 1; }
    }  

    return 0;
}

u8 get_op_precedence(char a)
{

    // loop through all operators
    for (int x = 0; x < num_ops*2; x += 2)
    {

        // when a match is found, return the precedence level 
        // of the operator
        if (a == operatorsOld[x]) { return atoi(&operatorsOld[x+1]); }
    }

    // no operator was found, return 0
    return 0;
}

char has_precedence(char a, char b)
{
    u8 a_prec = get_op_precedence(a);
    u8 b_prec = get_op_precedence(b);

    return a_prec >= b_prec;
}


u8 tokenize(char* expression, char mathstring[MATHSTR_LEN][MAX_NUM_LEN])
{
    // 9-9-24
    // Accepts a string with a mathematical expression and breaks 
    // it into smaller pieces for the parser. Outputs a 2D array 
    // with each individual part of the given expression. The expression 
    // is split up based upon operators

    // setup a few things
    char current[MAX_NUM_LEN+3];
    u8 output_index = 1;
    u8 current_len = 0;
    u8 expression_len = strlen(expression);

    // loop through the given expression
    for (int x = 0; x < expression_len; x++)
    {

        // if an operator is found, put current into
        // its own section in the output
        if (is_operator(expression[x]))
        {
            // copy over current and the current operator
            if (current_len == 0)
            {
                strncpy(mathstring[output_index], &expression[x], 1);
                output_index -= 1;
            }
            else
            {
                // make sure the number isn't too big
                if (current_len > MAX_NUM_LEN) { current_len = MAX_NUM_LEN; }

                // copy everything over
                snprintf(mathstring[output_index], MAX_NUM_LEN, "%.*s", current_len, current);
                strncpy(mathstring[output_index+1], &expression[x], 1);
            }

            // reset some things
            output_index += 2;
            current_len = 0;
        }

        // otherwise, add the current character to current
        else
        {
            // only add the character if current has room left
            if (current_len < MAX_NUM_LEN)
            {
                current[current_len] = expression[x];
                current_len += 1;
            }
            
            if (current_len >= MAX_NUM_LEN)
            {
                current[MAX_NUM_LEN-1] = '\0';
            }
        }

    }

    // append the final current to the output
    if (current_len >= MAX_NUM_LEN) { current_len = MAX_NUM_LEN; }
    snprintf(mathstring[output_index], MAX_NUM_LEN, "%.*s", current_len, current);

    // store the number of elements in the output in the output
    sprintf(mathstring[0], "%d", output_index);

    // it worked (hopefully)
    return 0;
    

}

char is_valid_number(char* number)
{
    // 9-9-24
    // Checks whether or not the given string is a number

    u8 len = strlen(number);
    
    // return if empty
    if (len <= 0) { return 0; }

    // // return if the first thing isn't a minus or a number
    // if (number[0] != '-' && !isdigit((unsigned char)number[0])) { return 0; }

    // check everything else
    for (int x = 0; x < len; x++)
    {
        if (!isdigit((unsigned char)number[x])) { return 0; }
    }

    // if here, it's a number
    return 1;

}

u8 parse(char mathstring[MATHSTR_LEN][MAX_NUM_LEN], char parsed[MATHSTR_LEN][MAX_NUM_LEN])
{

    // 9-9-24
    // Uses the shunting yard algorithm to turn an infix math 
    // expression (mathstring) into the same expression represented 
    // in postfix notation. You can get a mathstring from the tokenize 
    // function.

    char op_stack[MATHSTR_LEN];
    u8 parsed_ind = 1;
    u8 op_stack_top = 0;
    
    int flag = 0;

    // loops through each thing in the expression
    for (int x = 1; x < atoi(mathstring[0]) + 1; x++)
    {

        // if it's a number, add it to the output
        if (is_valid_number(mathstring[x])) 
        { 
            strcpy(parsed[parsed_ind], mathstring[x]); 
            parsed_ind += 1;

        }

        // if it's an operator
        else if ( (is_operator(mathstring[x][0])) && (mathstring[x][0] != '(') && (mathstring[x][0] != ')') )
        {

            // while the thing at the top of the op stack has greater 
            // precedence than the current op
            while ( (op_stack_top != 0) && (has_precedence(op_stack[op_stack_top], mathstring[x][0])) 
                     && (op_stack[op_stack_top] != '(') )
            {

                // remove things from the operator stack and put 
                // them in the output
                parsed[parsed_ind][0] = op_stack[op_stack_top];
                parsed[parsed_ind][1] = '\0';
                op_stack_top -= 1;
                parsed_ind += 1;

                flag += 1;
            }

            // add the current operator to the stack
            op_stack_top += 1;
            op_stack[op_stack_top] = mathstring[x][0];
                            
        }

        // if open parenthesis
        else if (mathstring[x][0] == '(')
        {
            // add it to the stack like usual
            op_stack_top += 1;
            op_stack[op_stack_top] = mathstring[x][0];
        }

        // if close parenthesis
        else if (mathstring[x][0] == ')')
        {

            // remove everything from the op stack until you 
            // reach an open parenthesis    
            while (op_stack[op_stack_top] != '(')
            {
                parsed[parsed_ind][0] = op_stack[op_stack_top];
                parsed[parsed_ind][1] = '\0';

                op_stack_top -= 1;
                parsed_ind += 1;
            }

            // if the current thing at the top of the stack isn't an 
            // open parenthesis, complain
            if (op_stack[op_stack_top] != '(')
            {
                char weenres[80];
                u8 peen = 15;
                sprintf(weenres, "s000000000000 angry: %c", op_stack[op_stack_top]);
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
        parsed[parsed_ind][0] = op_stack[op_stack_top];
        parsed[parsed_ind][1] = '\0';

        parsed_ind += 1;
        op_stack_top -= 1;
    }

    // put the final number of elements in the expression 
    // into the beginning of it
    sprintf(parsed[0], "%d", parsed_ind);

    // worked without error
    return 0;
}

struct fraction evaluate_parsedstring(char parsed[MATHSTR_LEN][MAX_NUM_LEN])
{
    // 9-9-24
    // evaluates a postfix expression and returns an actual result 
    // in the form of a fraction *sunglasses emoji*

    struct fraction stack[MATHSTR_LEN];
    u8 top_stack = 0;

    // loops through each element of the expression
    for (int x = 1; x < atoi(parsed[0]); x++)
    {

        // if the current element is an operator, evaluate it using the 
        // last two numbers in the stack
        if (is_operator(parsed[x][0]))
        {
            // pop last two things off stack
            struct fraction b = stack[top_stack-1];
            struct fraction a = stack[top_stack-2];

            top_stack -= 2;

            // do the calculation
            if (!strcmp(parsed[x], "+")) { a = fraction_add(a, b); }
            else if (!strcmp(parsed[x], "-")) { a = fraction_sub(a, b); }
            else if (!strcmp(parsed[x], "*")) { a = fraction_mul(a, b); }
            else if (!strcmp(parsed[x], "^")) { a = fraction_basic_power(a, b); }
            else if (!strcmp(parsed[x], "/")) 
            {

                // flip b (whatever it may be)
                b = fraction_reciprocal(b);
                
                // b is now the reciprocal of b, multiply by a
                a = fraction_mul(a, b);

                // simplify
                a = fraction_simplify(a); 
            }

            else 
            { 
                struct fraction a;
                a.sign = 0;  // this should never happen, so if it does, there's an error
                return a;
            }

            // put the result at the top of the stack
            stack[top_stack] = a;
            top_stack += 1;

        }
        else
        {

            // if not an operator, just add to the stack
            // makes a fraction from the numbers in the stack
            stack[top_stack] = fraction_init(str_to_u64(parsed[x]), 1, 1);
            top_stack += 1;

        }

    }
    
    // return the result
    return stack[top_stack-1];

}