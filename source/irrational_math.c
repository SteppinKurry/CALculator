// 9-30-24

#include "calmath.h"
#include <stdio.h>
#include <strings.h>

struct fraction psqrt_help(struct rat_matrix base, struct rat_matrix not_base, u8 reps)
{

    for (int x = 0; x < reps; x++)
    {
        not_base = rat_mat_multiply(base, not_base);
    }

    struct fraction final = fraction_div(not_base.elements[rowcol_to_index(0, 0)], 
                                        not_base.elements[rowcol_to_index(1, 0)]);

    return final;
}

struct fraction psqrt(struct fraction a, struct fraction b)
{

    if (a.sign == -1) { return fraction_init_error(); }
    return double_to_frac(sqrt(a.numerator/a.denominator));

    struct rat_matrix base = rat_matrix_init(2, 2);
    struct fraction nums[4] = {fraction_init(1, 1, 1), 
                                a, fraction_init(1, 1, 1), fraction_init(1, 1, 1)};
    matrix_fill(&base, nums);

    struct rat_matrix not_base = rat_matrix_init(2, 1);
    
    struct fraction peen[2] = {fraction_init(1, 1, 1), fraction_init(1, 1, 1)};
    matrix_fill(&not_base, peen);

    for (int x = 0; x < 99; x++)
    {
        not_base = rat_mat_multiply(base, not_base);
    }

    struct fraction final = fraction_init(not_base.elements[(rowcol_to_index(0, 0))].numerator, 
                                        not_base.elements[rowcol_to_index(1, 0)].numerator, 1);

    return final;

}

struct fraction fraction_sin(struct fraction a, struct fraction b)
{

    return double_to_frac(sin(fraction_mag(a)));
}

struct fraction fraction_cos(struct fraction a, struct fraction b)
{

    return double_to_frac(cos(fraction_mag(a)));
}

struct fraction fraction_tan(struct fraction a, struct fraction b)
{

    return double_to_frac(tan(fraction_mag(a)));
}