// 10-3-24
// This was moved to its own .c file during the 
// Great Refactoring

#include "calmath.h"
#include "ui.h"
#include <stdio.h>

struct fraction fraction_init(u64 num, u64 den, int8 sign)
{

    struct fraction a;
    a.numerator = num;
    a.denominator = den;
    a.sign = sign;

    return a;

}

struct fraction fraction_reciprocal(struct fraction a)
{
    if (a.numerator == 0) { return a; }
    
    u64 temp = a.numerator;

    a.numerator = a.denominator;
    a.denominator = temp;
    
    return a;

}

struct fraction fraction_simplify(struct fraction a)
{
    // 2-12-24
    // simplifies a  fraction using the (currently very basic) 
    // gcd algorithm and two integer divides

    // find the gcd between the numerator and denominator
    u64 greatestcd = gcd(a.numerator, a.denominator);

    // divide numerator and denominator by the gcd
    a.numerator = div64(a.numerator, greatestcd);
    a.denominator = div64(a.denominator, greatestcd);

    return a;
}

bool fractions_equal(struct fraction a, struct fraction b)
{

    a = fraction_simplify(a);
    b = fraction_simplify(b);

    bool are_equal = true;

    if (a.numerator != b.numerator) { are_equal = false; }
    if (a.denominator != b.denominator) { are_equal = false; }
    if (a.sign != b.sign) { are_equal = false; }

    return are_equal;
}

struct fraction fraction_add(struct fraction a, struct fraction b)
{
    // 2-12-24
    // adds two fractions together 

    // simplifies both fractions
    a = fraction_simplify(a);
    b = fraction_simplify(b);

    // calculate the lcm and use it as the denominator for 
    // the final answer
    u64 new_denom = lcm(a.denominator, b.denominator);
    
    // determine what to multiply the numerator by based upon 
    // the lcm/new denominator
    u64 delta_a = div64(new_denom, a.denominator);
    u64 delta_b = div64(new_denom, b.denominator);
    
    u64 new_a = a.numerator * delta_a;
    u64 new_b = b.numerator * delta_b;

    // if signs are the same
    if (a.sign == b.sign)
    {
        // add and multiply; sign is already correct
        a.numerator = new_a + new_b;

    }
    else if (a.sign == -1) // signs are different; a is negative
    {
        // subtract b from a and take absolute value
        a.numerator = abs( new_a - new_b );

        // figure out which sign is appropriate
        if (new_a >= new_b) { a.sign = -1; }
        else { a.sign = 1; }
    }
    else // b is negative
    {
        // subtract b from a and take absolute value
        a.numerator = abs( new_a - new_b );

        // figure out which sign is appropriate
        if (new_a <= new_b) { a.sign = -1; }
        else { a.sign = 1; }

        if (a.numerator == 0) { a.sign = 1; }
    }

    // denominator is the same regardless
    a.denominator = new_denom;

    a = fraction_simplify(a);
    return a;

}

struct fraction fraction_sub(struct fraction a, struct fraction b)
{
    b.sign = -1;

    return fraction_add(a, b);
}

struct fraction fraction_mul(struct fraction a, struct fraction b)
{
    a.numerator *= b.numerator;
    a.denominator *= b.denominator;

    a.sign *= b.sign;

    a = fraction_simplify(a);

    return a;
}

struct fraction fraction_div(struct fraction a, struct fraction b)
{
    b = fraction_reciprocal(b);

    return fraction_mul(a, b);
}

struct fraction fraction_basic_power(struct fraction a, struct fraction bi)
{

    // 10-11-24
    // Back again with the smooth-brain power functions
    u64 b = bi.numerator;
    
    struct fraction result = fraction_init(1, 1, 1);

    for (int x = 0; x < b; x++)
    {
        result = fraction_mul(result, a);
    }

    return result;


}

struct fraction fraction_none(struct fraction a, struct fraction b)
{
    return a;
}