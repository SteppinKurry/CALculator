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
    a.sci_notation = 0;

    return a;

}

struct fraction fraction_init_int(int64 num, int64 den)
{
    // 11-13-24
    
    struct fraction a;
    a.numerator = abs(num);
    a.denominator = abs(den);

    int8 num_sign = 1;
    int8 den_sign = 1;

    if (num < 0) { num_sign = -1; }
    if (den < 0) { den_sign = -1; }

    a.sign = num_sign *= den_sign;

    a.sci_notation = 0;

    return a;

}

double fraction_mag(struct fraction a)
{
    return (double) a.numerator / a.denominator * a.sign;
}

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

    return a;
}

struct fraction fraction_to_sci(struct fraction a, int64 places)
{
    // 11-10-24
    // Tries to represent (probably overflowing) fractions *intelligently* 
    // by converting to scientific notation. Tries to minimize precision 
    // loss by being *smart* with the numerator and denominator

    // does not currently do anything differently if you put in a negative 
    // number for places

    if (places == 0) { places = 3; }

    u64 power_of_ten = pow(10, abs(places));

    bool can_manipulate_num = false;
    bool can_manipulate_den = false;

    // see if numerator is greater than 10^places. If it isn't then we probably 
    // shouldn't take anything from it
    if (a.numerator >= power_of_ten) { can_manipulate_num = true; }

    // check the denominator in a similar way
    if (a.denominator >= power_of_ten) { can_manipulate_den = true; }

    // if we can manipulate both, just take the power of 10 away
    if (can_manipulate_num && can_manipulate_den)
    {
        a.numerator /= power_of_ten;
        a.denominator /= power_of_ten;
        
        // since the top and bottom are changing by the same factor, 
        // don't change the current scientific notation

    }

    else if (can_manipulate_num)
    {
        // then reduce the numerator and note it in the 
        // scientific_notation

        a.numerator /= power_of_ten;

        a.sci_notation += places;

    }
    else if (can_manipulate_den)
    {
        // then reduce the denominator and note it in the 
        // scientific_notation

        a.denominator /= power_of_ten;

        a.sci_notation -= places;
    }

    return a;
    
}

bool fractions_equal(struct fraction a, struct fraction b)
{
    // 10-17-24
    // Returns true if the two given fractions are equal
    a = fraction_simplify(a);
    b = fraction_simplify(b);

    if (a.numerator != b.numerator) { return false; }
    if (a.denominator != b.denominator) { return false; }
    if (a.sign != b.sign) { return false; }
    if (a.sci_notation != b.sci_notation) { return false; }

    return true;
}

struct fraction fraction_add(struct fraction a, struct fraction b)
{
    // 11-10-24
    // adds two fractions together 

    // simplifies both fractions
    a = fraction_simplify(a);
    b = fraction_simplify(b);

    // detect and prevent overflow
    bool numerator_overflow = a.numerator > __UINT64_MAX__ - b.numerator;
    bool denominator_overflow = a.denominator > __UINT64_MAX__ - b.denominator;

    while (numerator_overflow || denominator_overflow)
    {
        a = fraction_to_sci(a, 3);
        b = fraction_to_sci(b, 3);

        numerator_overflow = a.numerator > __UINT64_MAX__ - b.numerator;
        denominator_overflow = a.denominator > __UINT64_MAX__ - b.denominator;

    }


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

    // these are true if there's overflow
    bool numerator_overflow = a.numerator > __UINT64_MAX__ / b.numerator;
    bool denominator_overflow = a.denominator > __UINT64_MAX__ / b.denominator;

    while (numerator_overflow || denominator_overflow)
    {
        // a and b are close to each other, so reduce both
        if ((fraction_mag(a) - fraction_mag(b)) < 1000)
        {
            a = fraction_to_sci(a, 3);
            b = fraction_to_sci(b, 3);
        }

        // otherwise, reduce the (much larger) one
        else if (fraction_mag(a) > fraction_mag(b)) { a = fraction_to_sci(a, 3); }
        else { b = fraction_to_sci(b, 3); }

        numerator_overflow = a.numerator > __UINT64_MAX__ / b.numerator;
        denominator_overflow = a.denominator > __UINT64_MAX__ / b.denominator;

    }


    a.numerator *= b.numerator;

    a.denominator *= b.denominator;
    a.sign *= b.sign;

    a.sci_notation += b.sci_notation;

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
    if (a.numerator == 0 && a.denominator == 0 && a.sign == 0)
    {
        return true;
    }

    return false;
}

struct fraction fraction_init_error()
{
    // 10-17-24
    // Generates an error fraction

    return fraction_init(0, 0, 0);
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