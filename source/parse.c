// 3-18-24

#include "parse.h"


struct bigreal basic_do_math(struct tokenized wang)
{
    // 3-18-24

    if (wang.operation == '+') { return  bigreal_add(wang.first, wang.second); }
    else if (wang.operation == '-')
    {
        wang.second.sign = -1;
        return bigreal_add(wang.first, wang.second);
    }
    else if (wang.operation == '*') { return bigreal_multiply(wang.first, wang.second); }
    else if (wang.operation == '/') 
    { 
        struct bigreal a;
        a.numerator = shit_div64(wang.first.numerator, wang.second.numerator);
        a.denominator = 1;
        return a;
    }

    // Because of the nature of the parse function, the only 
    // bigreal that's filled when there is no operator is the 
    // second one. The other one defaults to 0
    return wang.second;


}

int parse(char* expression, struct tokenized* output)
{

    struct bigreal first;
    struct bigreal second;

    first.denominator = 1;
    first.sign = 1;

    second.denominator = 1;
    second.sign = 1;

    u64 first_numer = 0;
    u64 second_numer = 0;

    char idx = 0;
    char state = 0;

    for (int x = (strlen(expression) - 1); x >= 0; x--)
    {
        if (state == 0)
        {
            if (expression[x] >= '0' && expression[x] <= '9')
            {
                second_numer += (expression[x] - '0') * power(10, idx);
                idx += 1;
            }
            else
            {
                output->operation = expression[x];
                state = 1;
                idx = 0;
            }

        }

        if (state == 1)
        {
            if (expression[x] >= '0' && expression[x] <= '9')
            {
                first_numer += (expression[x] - '0') * power(10, idx);
                idx += 1;
            }

        }
    }

    first.numerator = first_numer;
    second.numerator = second_numer;

    output->first = first;
    output->second = second;

    return 1;
}

