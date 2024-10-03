// 9-30-24
// this is where all of the math functions that can result in irrational 
// numbers (sqrt, log, trig, etc.) reside

#ifndef IRRATIONALMATH
#define IRRATIONALMATH

#include <nds.h>

struct bigreal root(struct bigreal a, u64 index);

#endif