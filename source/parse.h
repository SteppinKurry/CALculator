// 3-18-24
// this takes care of breaking up expressions into 
// something the code can actually work with

#ifndef BREAKUPS
#define BREAKUPS

#include <string.h>
#include "calmath.h"

// struct tokenized
// {
//     struct bigreal first;
//     struct bigreal second;

//     char operation;
// };


struct tokenized
{
    struct bigreal first;
    struct bigreal second;

    char operation;
};

int parse(char* expression, struct tokenized* output);

struct bigreal basic_do_math(struct tokenized wang);


#endif