// 1-29-24
// trying to implement really fast math functions (even 
// though libnds probably already has them)

#include <calmath.h>

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

    // add and multiplys
    a.numerator = (a.numerator * delta_a) + (b.numerator * delta_b);
    a.denominator = new_denom;

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
    return div64(a * b, gcd(a, b));
}

