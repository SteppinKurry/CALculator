// 10-3-24
// This was moved to its own .c file during the 
// Great Refactoring

#include "calmath.h"

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