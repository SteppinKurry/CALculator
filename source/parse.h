// 3-18-24
// this takes care of breaking up expressions into 
// something the code can actually work with

#ifndef TIMETOPARSE
#define TIMETOPARSE

#include "calmath.h"
#include "sizes.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

u8 parse(char mathstring[MATHSTR_LEN][MAX_NUM_LEN], char parsed[MATHSTR_LEN][MAX_NUM_LEN]);
u8 tokenize(char* expression, char mathstring[50][MAX_NUM_LEN]);
bool is_operator(char* e);
u8 get_op_precedence(char* a);
bool is_valid_number(char* number);
bool a_has_precedence(char* a, char* b);
struct fraction evaluate_parsedstring(char parsed[MATHSTR_LEN][MAX_NUM_LEN]);
bool is_math_func(enum operators op);
enum operators str_to_op(char* c);

bool is_user_var(char* str);
u8 user_var_to_int(char* str);
bool is_math_const(char* str);

//struct bigreal basic_do_math(struct tokenized wang);

#endif