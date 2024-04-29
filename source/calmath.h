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
    int8 sign;  // 1 if positive, -1 if negative
};

struct bigreal bigreal_add(struct bigreal a, struct bigreal b);
struct bigreal bigreal_simplify(struct bigreal a);
struct bigreal bigreal_multiply(struct bigreal first, struct bigreal second);

u64 shit_div64(u64 a, u64 b);
u64 power(u64 base, u64 exponent);
u64 gcd(u64 a, u64 b);
u64 lcm(u64 a, u64 b);

u64 bit_set_to(u64 number, u64 n, bool x);
bool bit_check(u64 number, u64 n);

#endif