// 1-29-24
// All of the math-related functions are declared here.

#ifndef QUICKMATHS
#define QUICKMATHS

#include <nds.h>
#include "sizes.h"

// ==================================================================
// very important structs
// ==================================================================

enum operators
{

    NOOP = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    POW = 5,
    LPAREN = 6,
    RPAREN = 7,
    SIN = 8,
    COS = 9,
    TAN = 10,
    LOG = 11,
    E = 12,
    SQRT = 13

};

struct fraction
{
    // pretty straightforward
    // ( -1/2 )
    u64 numerator;
    u64 denominator;
    int8 sign;
};

struct fraction_exponent
{
    // a fraction to the power of another fraction
    // ( (5/3)^(3/2) )
    struct fraction base;
    struct fraction exponent;
};

// ==================================================================
// exponent math stuff
// ==================================================================

struct fraction_exponent real_exp_simplify(struct fraction_exponent a);


// ==================================================================
// irrational stuff
// ==================================================================

struct fraction root(struct fraction a, u64 index);


// ==================================================================
// basic fraction stuff
// ==================================================================
struct fraction fraction_init(u64 num, u64 den, int8 sign);
struct fraction fraction_reciprocal(struct fraction a);
struct fraction fraction_simplify(struct fraction a);
bool fractions_equal(struct fraction a, struct fraction b);

struct fraction fraction_none(struct fraction a, struct fraction b);
struct fraction fraction_add(struct fraction a, struct fraction b);
struct fraction fraction_sub(struct fraction a, struct fraction b);
struct fraction fraction_mul(struct fraction a, struct fraction b);
struct fraction fraction_div(struct fraction a, struct fraction b);
struct fraction fraction_basic_power(struct fraction a, struct fraction bi);


// ==================================================================
// unsimple stuff
// ==================================================================
struct unsimple_exp unsimple_exp_init();
struct node* unsimple_add_node(struct node n, struct unsimple_exp* tree);

int8 construct_unsimple_from_parsedstring(char parsed[MATHSTR_LEN][MAX_NUM_LEN], struct unsimple_exp* tree);
int8 unsimple_simplify(struct unsimple_exp* tree);
int8 unsimple_simplify_node(struct node* n);

int8 unsimple_combine_scalars(struct node* n);

int8 unsimple_rewrite(struct node* n);
int8 addition_rewrites(struct node* n);
int8 multiplication_rewrites(struct node* n);
int8 division_rewrites(struct node* n);

struct fraction unsimple_evaluate(struct unsimple_exp* tree);
struct fraction unsimple_eval_node(struct node* n);


// ==================================================================
// basic/general stuff
// ==================================================================
u64 power(u64 base, u64 exponent);
double power_double(double base, double exponent);
u64 gcd(u64 a, u64 b);
u64 lcm(u64 a, u64 b);
u64 str_to_u64(char* str);

u64 bit_set_to(u64 number, u64 n, bool x);
bool bit_check(u64 number, u64 n);

long power_long(long base, long exponent);

#endif
