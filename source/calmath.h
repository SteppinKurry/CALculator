// 1-29-24
// trying to implement really fast math functions (even 
// though libnds probably already has them)

#ifndef QUICKMATHS
#define QUICKMATHS

#include <nds.h>

struct bigreal
{
    u64 numerator;
    u64 denominator;
    char negative;
};

struct bigreal bigreal_add(struct bigreal a, struct bigreal b);
struct bigreal bigreal_simplify(struct bigreal a);
u64 gcd(u64 a, u64 b);
u64 lcm(u64 a, u64 b);

#endif