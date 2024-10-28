// 10-3-24
// This was moved to its own .c file during the 
// Great Refactoring

#include "calmath.h"
#include "ui.h"
#include <stdio.h>

struct fraction fraction_init(u64 num, u64 den, int8 sign)
{

    struct fraction a;
    a.whole = 0;
    a.numerator = num;
    a.denominator = den;
    a.sign = sign;
    a.sci_notation = 1;

    return a;

}

struct fraction fraction_init_whole(u64 whole, u64 num, u64 den, int8 sign)
{
    struct fraction a;
    a.whole = whole;
    a.numerator = num;
    a.denominator = den;
    a.sign = sign;
    a.sci_notation = 1;

    return a;

}

///  //////////////////
struct fraction fraction_reciprocal(struct fraction a)
{
    if (a.numerator == 0) { return a; }
    
    u64 temp = a.numerator;

    a.numerator = a.denominator;
    a.denominator = temp;

    a.sci_notation *= 1;
    
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
    a.numerator = a.numerator / greatestcd;
    a.denominator = a.denominator / greatestcd;

    // check to see if the whole part can be added back into 
    // the fractional part

    return a;
}

struct fraction fraction_make_mixed(struct fraction a)
{

    // simplify the fraction
    a = fraction_simplify(a);

    // set the whole number component
    a.whole += a.numerator / a.denominator;
    
    // modify the fractional component
    a.numerator = a.numerator % a.denominator;

    return a;
}

bool fractions_equal(struct fraction a, struct fraction b)
{
    // 10-17-24
    // Returns true if the two given fractions are equal
    a = fraction_simplify(a);
    b = fraction_simplify(b);

    if (a.whole != b.whole) { return false; }
    if (a.numerator != b.numerator) { return false; }
    if (a.denominator != b.denominator) { return false; }
    if (a.sign != b.sign) { return false; }

    return true;
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
    u64 new_whole = 0;

    // if signs are the same
    if (a.sign == b.sign)
    {
        // add and multiply; sign is already correct
        a.numerator = new_a + new_b;
        new_whole  = a.whole + b.whole;

    }
    else if (a.sign == -1) // signs are different; a is negative
    {
        // subtract b from a and take absolute value
        a.numerator = abs(new_a - new_b);
        new_whole = abs(a.whole - b.whole);

        // figure out which sign is appropriate
        // start with the whle numbers
        if (a.whole > b.whole) { a.sign = -1; }
        else { a.sign = 1; }

        // if the whole numbers are the same, check the fractions
        if (a.whole == b.whole)
        {
            if (new_a >= new_b) { a.sign = -1; }
            else { a.sign = 1; }
        }

    }
    else // b is negative
    {
        // subtract b from a and take absolute value
        a.numerator = abs(new_a - new_b);
        new_whole = abs(a.whole - b.whole);

        // figure out which sign is appropriate
        // start with the whle numbers
        if (a.whole > b.whole) { a.sign = 1; }
        else { a.sign = -1; }

        // if the whole numbers are the same, check the fractions
        if (a.whole == b.whole)
        {
            if (new_a <= new_b) { a.sign = -1; }
            else { a.sign = 1; }
        }

        if (a.numerator == 0) { a.sign = 1; }
    }

    // denominator is the same regardless
    a.denominator = new_denom;

    a.whole = new_whole;

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
    a = fraction_simplify(a);
    b = fraction_simplify(b);

    // these are true if there's overflow
    bool numerator_overflow = a.numerator > __UINT64_MAX__ / b.numerator;
    bool denominator_overflow = a.denominator > __UINT64_MAX__ / b.denominator;

    if (numerator_overflow || denominator_overflow)
    {
        // make mixed fractions out of each number before proceeding
        a = fraction_make_mixed(a);
        b = fraction_make_mixed(b);

        char ween[200];
        sprintf(ween, "%llu+%llu/%llu", b.whole, b.numerator, b.denominator);
        annoyed_print(ween);
    }

    // multiply the whole number parts independently
    a.whole *= b.whole;

    // calculate the fractional part
    a.numerator =   (a.whole * b.numerator * a.denominator) + 
                    (a.numerator * b.whole * b.denominator) + 
                    (a.numerator * b.numerator);

    a.denominator *= b.denominator;

    // calculate sign
    a.sign *= b.sign;

    return fraction_simplify(a);
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
    // doesn't do anything, but I need it for the array of all 
    // fraction functions
    return a;
}

bool fraction_is_error(struct fraction a)
{
    // 10-17-24
    // An "error fraction" indicates weird and should never occur 
    // on accident. Therefore, if one is detected, something weird/wrong/different 
    // is probably supposed to happen. An error fraction has a 0 in every field.
    if (a.whole == 0 && a.numerator == 0 && a.denominator == 0 && a.sign == 0)
    {
        return true;
    }

    return false;
}

struct fraction fraction_init_error()
{
    // 10-17-24
    // Generates an error fraction

    return fraction_init_whole(0, 0, 0, 0);
}

struct fraction double_to_frac(double a)
{
    // 10-17-24
    // Turns a double into a fraction by first converting it to a 
    // string, counting the digits after the decimal place, turning the 
    // original number into a u64 and using it as the numerator, and using 
    // the appropriate power of 10 as the denominator

    // make the number string
    char numstr[20];
    sprintf(numstr, "%0.7f", a);

    u8 peen = 10;
    calc_main_print(numstr, &peen, 0);

    // count the number of digits after the decimal place
    u8 numstrlen = strlen(numstr);
    u8 digits_after_decimal = 0;
    bool counting_digits = false;

    for (int x = 0; x < numstrlen; x++)
    {
        if (counting_digits)
        {
            digits_after_decimal += 1;
        }

        if (numstr[x] == '.') { counting_digits = true; }

    }

    // determine the appropriate sign
    int8 sign = -1;
    if (a >= 0) { sign = 1; }

    // calculate the denominator
    u64 denominator = power_double(10, digits_after_decimal);

    // generate the fraction and simplify it
    return fraction_simplify(fraction_init(ceil(fabs(a) * denominator), denominator, sign));
}