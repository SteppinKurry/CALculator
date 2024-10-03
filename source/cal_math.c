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

struct bigreal bigreal_sub(struct bigreal a, struct bigreal b)
{
    // 9-30-24 
    // I got sick of making b's sign -1 first

    b.sign = -1;
    
    return bigreal_add(a, b);
}

struct bigreal bigreal_multiply(struct bigreal a, struct bigreal b)
{
    a.numerator *= b.numerator;
    a.denominator *= b.denominator;

    a.sign *= b.sign;

    a = bigreal_simplify(a);

    return a;
}

struct bigreal bigreal_root(struct bigreal a, u64 index)
{

    if (index == 1) { return a; }

    a.function = ROOT;
    a.func_arguments[0] = index;
    a.num_args = 1;

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
    a.numerator = div64(a.numerator, greatestcd);
    a.denominator = div64(a.denominator, greatestcd);

    return a;
}

struct bigexp simplify_bigexp(struct bigexp)
{
    // 10-3-24
    // Puts an expression of bigreals into its simplest form 
    // without evaluating any irrational functions

    


}

struct bigreal bigreal_reciprocal(struct bigreal a)
{
    // 9-30-24
    // Makes dividing easier

    u64 temp = a.numerator;

    a.numerator = a.denominator;
    a.denominator = temp;
    
    return a;

}

struct bigreal bigreal_basic_power(struct bigreal a, u64 power)
{
    // 9-30-24
    // Another smooth-brain implementation, but this time extended 
    // to bigreals. Only capable of whole number powers

    struct bigreal return_this = bigreal_init(1, 1, a.sign);

    for (int x = 0; x < power; x++)
    {
        return_this = bigreal_multiply(return_this, a);
    }
    
    return return_this;

}

struct bigreal bigreal_init(u64 numerator, u64 denominator, int8 sign)
{
    // 9-28-24
    // A function to make a bigreal. I wanted this in case I need to 
    // change the implementation of a bigreal and didn't want to change 
    // it everywhere else

    struct bigreal return_this;
    return_this.numerator = numerator;
    return_this.denominator = denominator;
    return_this.sign = sign;
    return_this.function = 0;
    return_this.num_args = 0;

    return return_this;
}

bool bigreal_is_greater_than(struct bigreal a, struct bigreal b)
{
    // 9-30-24
    // Tests whether or not a > b and returns the result.

    // subtract the two numbers
    b.sign = -1;
    struct bigreal ans = bigreal_add(a, b);

    // if the result is 0, they were equal
    if (ans.numerator == 0) { return false; }

    // if the result is negative, b > a
    if (ans.sign == -1) { return false; }
    
    // if neither of those, a > b
    return true;
}

bool bigreal_is_equal_to(struct bigreal a, struct bigreal b)
{
    // 9-30-24
    // Tests whether or not a == b and returns the result.

    // subtract the two numbers
    b.sign = -1;
    struct bigreal ans = bigreal_add(a, b);

    // if the result is 0, they were equal
    if (ans.numerator == 0) { return true; }

    return false;
}

u64 str_to_u64(char* str)
{
    // 9-26-24
    // converts a number stored as a string to an actual number
    // I needed this because atoi casts to an int and not a u64
    u64 return_this = 0;
    u8 len_str = strlen(str);
    u8 digits = len_str-1;

    for (int x = 0; x < len_str; x++)
    {
        return_this += (str[x] - '0') * power(10, digits);
        digits -= 1;
    }

    return return_this;
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

double power_double(double base, double exponent)
{
    // 9-30-24
    // I'm quite annoyed

    double result = 1;
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

// u64 shit_div64(u64 a, u64 b)
// {
//     // 3-25-24
//     // found this algorithm on Wikipedia, and it seems 
//     // to be fast enough for right now

//     // currently only supports integer division
//     u64 quotient = 0;
//     u64 remainder = 0;

//     for (int x = 63; x >= 0; x--)
//     {
//         remainder = remainder << 1;
//         remainder = bit_set_to(remainder, 0, bit_check(a, x));

//         if (remainder >= b)
//         {
//             remainder -= b;
//             quotient = bit_set_to(quotient, x, 1);
//         }
//     }

//     return quotient;
// }
