// 1-29-24
// trying to implement really fast math functions (even 
// though libnds probably already has them)

#ifndef QUICKMATHS
#define QUICKMATHS

#include <nds.h>
#include "irrational_math.h"

enum irr_function
{
    NONE = 0,
    ROOT = 1,
    LOG = 2,
    SIN = 3,
    COS = 4,
    TAN = 5
};

struct bigreal // tehcnically just a bigrational rn
{
    u64 numerator;
    u64 denominator;
    int8 sign;  // 1 if positive, -1 if negative
    enum irr_function function; // the "id" for the function acting upon number, 0 for no function

    u64 func_arguments[5]; // represents any arguments to the function
    u8 num_args;
};

struct bigexp
{
    struct bigreal expressions[15];
    
    u8 num_exp;
};

struct bigreal bigreal_add(struct bigreal a, struct bigreal b);
struct bigreal bigreal_sub(struct bigreal a, struct bigreal b);
struct bigreal bigreal_simplify(struct bigreal a);
struct bigreal bigreal_multiply(struct bigreal first, struct bigreal second);
struct bigreal bigreal_basic_power(struct bigreal a, u64 power);
struct bigreal bigreal_init(u64 numerator, u64 denominator, int8 sign);

struct bigreal bigreal_reciprocal(struct bigreal a);
bool bigreal_is_greater_than(struct bigreal a, struct bigreal b);
bool bigreal_is_equal_to(struct bigreal a, struct bigreal b);

u64 power(u64 base, u64 exponent);
double power_double(double base, double exponent);
u64 gcd(u64 a, u64 b);
u64 lcm(u64 a, u64 b);
u64 str_to_u64(char* str);

u64 bit_set_to(u64 number, u64 n, bool x);
bool bit_check(u64 number, u64 n);

long power_long(long base, long exponent);

#endif

// u64 shit_div64(u64 a, u64 b);
