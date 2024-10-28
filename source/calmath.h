// 1-29-24
// All of the math-related functions are declared here.

#ifndef QUICKMATHS
#define QUICKMATHS

#include <nds.h>
#include <math.h>
#include "sizes.h"

// ==================================================================
// general stuff
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

// ==================================================================
// basic fraction stuff
// ==================================================================
struct fraction
{
    // pretty straightforward
    u64 whole;
    u64 numerator;
    u64 denominator;
    int8 sign;
    int64 sci_notation;
};

struct fraction fraction_init(u64 num, u64 den, int8 sign);
struct fraction fraction_init_whole(u64 whole, u64 num, u64 den, int8 sign);
struct fraction fraction_reciprocal(struct fraction a);
struct fraction fraction_make_mixed(struct fraction a);
struct fraction fraction_simplify(struct fraction a);
bool fractions_equal(struct fraction a, struct fraction b);

struct fraction fraction_none(struct fraction a, struct fraction b);
struct fraction fraction_add(struct fraction a, struct fraction b);
struct fraction fraction_sub(struct fraction a, struct fraction b);
struct fraction fraction_mul(struct fraction a, struct fraction b);
struct fraction fraction_div(struct fraction a, struct fraction b);
struct fraction fraction_basic_power(struct fraction a, struct fraction bi);

bool fraction_is_error(struct fraction a);
struct fraction fraction_init_error();
struct fraction double_to_frac(double a);

// ==================================================================
// unsimple stuff
// ==================================================================
struct unsimple_exp unsimple_exp_init();
struct node* unsimple_add_node(struct node n, struct unsimple_exp* tree);

int8 unsimple_copy(struct unsimple_exp* source, struct unsimple_exp* dest);
int8 construct_unsimple_from_parsedstring(char parsed[MATHSTR_LEN][MAX_NUM_LEN], struct unsimple_exp* tree);
int8 unsimple_simplify(struct unsimple_exp* tree);
int8 unsimple_simplify_node(struct node* n);

int8 unsimple_combine_scalars(struct unsimple_exp* tree);
int8 unsimple_sub_vars(struct unsimple_exp* tree, struct unsimple_exp* values);

int8 unsimple_rewrite(struct node* n);
int8 addition_rewrites(struct node* n);
int8 multiplication_rewrites(struct node* n);
int8 division_rewrites(struct node* n);

struct fraction unsimple_evaluate(struct unsimple_exp* tree);
int8 unsimple_combine_scalars_node(struct node* n);
struct fraction unsimple_eval_node(struct node* n);

// ==================================================================
// matrix stuff
// ==================================================================
struct rat_matrix
{
    // a matrix with only rational numbers as elements
    u8 rows;
    u8 cols;

    struct fraction elements[RAT_MATR_MAX*RAT_MATR_MAX];

};

struct rat_matrix rat_matrix_init(u8 x_size, u8 y_size);
int8 matrix_fill(struct rat_matrix* mat, struct fraction* nums);

struct rat_matrix rat_mat_multiply(struct rat_matrix a, struct rat_matrix b);

struct rat_matrix matrix_init_error();
bool matrix_is_error(struct rat_matrix a);

u64 rowcol_to_index(u8 row, u8 col);


// ==================================================================
// irrational stuff
// ==================================================================
struct fraction root(struct fraction a, u64 index);
struct fraction psqrt(struct fraction a, struct fraction b);

// ==================================================================
// basic/general stuff
// ==================================================================
u64 power(u64 base, u64 exponent);
double power_double(double base, double exponent);
u64 gcd(u64 a, u64 b);
u64 lcm(u64 a, u64 b);
u64 str_to_u64(char* str);
bool is_commutative(enum operators op);
u64 bit_set_to(u64 number, u64 n, bool x);
bool bit_check(u64 number, u64 n);

long power_long(long base, long exponent);

#endif
