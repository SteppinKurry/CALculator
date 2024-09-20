// 1-29-24
// trying to implement really fast math functions (even 
// though libnds probably already has them)

#include "calmath.h"
#include "ui.h"
#include <stdio.h>

struct bigreal bigreal_add(struct bigreal a, struct bigreal b)
{   
    // 2-12-24
    // adds two bigreal fractions together 

    // simplifies both fractions
    a = bigreal_simplify(a);
    b = bigreal_simplify(b);

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

    a = bigreal_simplify(a);
    return a;
}

struct bigreal bigreal_multiply(struct bigreal a, struct bigreal b)
{
    a.numerator *= b.numerator;
    a.denominator *= b.denominator;

    a.sign *= b.sign;

    a = bigreal_simplify(a);

    return a;
}

struct bigreal bigreal_simplify(struct bigreal a)
{
    // 2-12-24
    // simplifies a bigreal fraction using the (currently very basic) 
    // gcd algorithm and two integer divides

    // find the gcd between the numerator and denominator
    u64 greatestcd = gcd(a.numerator, a.denominator);

    // divide numerator and denominator by the gcd
    a.numerator = shit_div64(a.numerator, greatestcd);
    a.denominator = shit_div64(a.denominator, greatestcd);

    return a;
}

u64 shit_div64(u64 a, u64 b)
{
    // 3-25-24
    // found this algorithm on Wikipedia, and it seems 
    // to be fast enough for right now

    // currently only supports integer division
    u64 quotient = 0;
    u64 remainder = 0;

    for (int x = 63; x >= 0; x--)
    {
        remainder = remainder << 1;
        remainder = bit_set_to(remainder, 0, bit_check(a, x));

        if (remainder >= b)
        {
            remainder -= b;
            quotient = bit_set_to(quotient, x, 1);
        }
    }

    return quotient;
}

u64 power(u64 base, u64 exponent)
{
    // 3-25-24
    // very smooth-brain implementation of exponents

    u64 result = 1;
    for (int x = 0; x < exponent; x++)
    {
        result *= base;
    }

    return result;
}

u64 gcd(u64 a, u64 b)
{
    // 2-12-24
    // an implementation of Euclid's greatest common denominator 
    // algorithm I stole from the computer organization class I took 
    // last year

    // no idea how it works really, and I don't particularly care 
    // right now

    u64 r;
    L0: r = mod64(a, b);
    if (r == 0) goto L1;
    a = b;
    b = r;
    goto L0;

    L1: return b;
}

u64 lcm(u64 a, u64 b)
{
    // 2-12-24
    // really basic implementation of a least common multiple 
    // "algorithm"

    return (a * b) / gcd(a, b);
}

u64 bit_set_to(u64 number, u64 n, bool x) 
{
    // 3-25-24
    // sets bit n of number to x and returns the result

    return (number & ~((u64)1 << n)) | ((u64)x << n);
}

bool bit_check(u64 number, u64 n) 
{
    // 3-25-24
    // returns the bit of number at position n

    return (number >> n) & (u64)1;
}