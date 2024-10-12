// 9-30-24

#include "calmath.h"
#include <stdio.h>
#include <strings.h>

struct fraction root(struct fraction a, u64 index)
{
    // 9-30-24
    // This uses the method of bisection to calculate the root 
    // of a fraction. The returned fraction will be an approximation 
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

    // convert the midpoint into a fraction
    u8 digits_after_decimal = 5;
    char buffer[32];

    snprintf(buffer, 32, "%f", midpoint);

    char* decimal_point = strchr(buffer, '.');
    digits_after_decimal = strlen(decimal_point);

    u64 large_ten = power(10, digits_after_decimal);
    midpoint *= large_ten;
    a = fraction_init(midpoint, 1, 1);

    return a;

}