// 1-29-24
// trying to implement really fast math functions (even 
// though libnds probably already has them)

#ifndef QUICKMATHS
#define QUICKMATHS

#include <nds.h>

struct bigreal // tehcnically just a bigrational rn
{
    u64 numerator;
    u64 denominator;
    int8 sign;  // 1 if positive, -1 if negative
    u16 function; // the "id" for the function acting upon number, 0 for no function
};

struct bigresult
{
    struct bigreal expressions[15];
    u8 length;
};

struct unsimplified_num
{
    struct bigreal number;
    u16 function;   // the "id" for the function acting upon number
};


struct bigreal bigreal_add(struct bigreal a, struct bigreal b);
struct bigreal bigreal_simplify(struct bigreal a);
struct bigreal bigreal_multiply(struct bigreal first, struct bigreal second);
struct bigreal bigreal_init(u64 numerator, u64 denominator, int8 sign);

u64 shit_div64(u64 a, u64 b);
u64 power(u64 base, u64 exponent);
u64 gcd(u64 a, u64 b);
u64 lcm(u64 a, u64 b);
u64 str_to_u64(char* str);

u64 bit_set_to(u64 number, u64 n, bool x);
bool bit_check(u64 number, u64 n);

long power_long(long base, long exponent);

#endif