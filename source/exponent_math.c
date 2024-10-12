// 10-4-24
// Functions relating to operations with fraction numbers to fraction number powers

#include "calmath.h"
#include <stdio.h>


// what if every weird number has a pointer to the function to simplify 
// them? They would still all have to result in the same type I think

// https://www.cuemath.com/algebra/exponent-rules/

struct fraction_exponent fraction_exp_simplify(struct fraction_exponent a)
{

    // zero exponent rule
    if (a.exponent.numerator == 0)
    {
        a.exponent = fraction_init(1, 1, 1);
        a.base = fraction_init(1, 1, 1);

        return a;
    }

    // negative exponent rule
    if (a.exponent.sign == -1)
    {
        a.exponent.sign = 1;
        a.base = fraction_reciprocal(a.base);

        return a;
    }

    // simplify each fraction
    a.base = fraction_simplify(a.base);
    a.exponent = fraction_simplify(a.exponent);

    return a;
}

// struct unsimple_exp fraction_exp_mul(struct fraction_exponent a, struct fraction_exponent b)
// {

//     struct unsimple_exp exp = unsimple_exp_init();

//     // product rule
//     if (fractions_equal(a.base, b.base))
//     {
//         a.exponent = fraction_add(a.exponent, b.exponent);

//         unsimple_add(exp, a);
//     }

    
//     return exp;

// }