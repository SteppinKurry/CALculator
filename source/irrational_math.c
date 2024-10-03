// 9-30-24

#include "irrational_math.h"
#include "calmath.h"
#include <stdio.h>
#include <strings.h>

struct bigreal root(struct bigreal a, u64 index)
{
    // 9-30-24
    // This uses the method of bisection to calculate the root 
    // of a bigreal. The returned bigreal will be an approximation 
    // of the root

    double divided = (double) a.numerator / a.denominator;
    double top = divided;

    if (top < 0) { top = 1.0; }

    double bottom = 0;
    double midpoint = 0.00001;

    // the root will be within this distance from the true answer
    double tolerance = 0.000001;

    // while the difference between the top and bottom isn't close enough
    while ((top - bottom) > tolerance)
    {

        // calculate the new midpoint
        midpoint = (top + bottom) / 2;

        // set the new bounds
        if (power_double(midpoint, index) > divided)
        {
            top = midpoint;
        }

        else
        {
            bottom = midpoint;
        }

    }

    // convert the midpoint into a bigreal
    u8 digits_after_decimal = 5;
    char buffer[32];

    snprintf(buffer, 32, "%f", midpoint);

    char* decimal_point = strchr(buffer, '.');
    digits_after_decimal = strlen(decimal_point);

    u64 large_ten = power(10, digits_after_decimal);
    midpoint *= large_ten;
    a = bigreal_init(midpoint, 1, 1);

    return a;


// struct bigreal root(struct bigreal a, u64 index)
// {
//     // 9-30-24
//     // This uses the method of bisection to calculate the root 
//     // of a bigreal. The returned bigreal will be an approximation 
//     // of the root

//     struct bigreal top = a;
//     struct bigreal bottom = bigreal_init(0, 1, 1);
//     struct bigreal midpoint = bigreal_init(0, 1, 1);

//     // the root will be within this distance from the true answer
//     struct bigreal tolerance = bigreal_init(1,  100000, 1);

//     // while the difference between the top and bottom isn't close enough
//     while (bigreal_is_greater_than(bigreal_sub(top, bottom), tolerance) )
//     {

//         // calculate the new midpoint
//         midpoint = bigreal_multiply(bigreal_add(top, bottom), bigreal_init(1, 2, 1));

//         if (bigreal_is_greater_than(bigreal_basic_power(midpoint, index), a))
//         {
//             top = midpoint;
//         }

//         else
//         {
//             bottom = midpoint;
//         }

//     }

//     return midpoint;




}
