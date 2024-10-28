// 10-23-24

#include "calmath.h"
#include <stdio.h>
#include "ui.h"

struct rat_matrix rat_matrix_init(u8 rows, u8 cols)
{
    struct rat_matrix a;

    a.rows = rows;
    a.cols = cols;

    for (int row = 0; row < a.rows; row++)
    {

        for (int col = 0; col < a.cols; col++)
        {
            a.elements[rowcol_to_index(row, col)] = fraction_init(0, 1, 1);
        }

    }

    return a;
}


struct rat_matrix rat_mat_multiply(struct rat_matrix a, struct rat_matrix b)
{
    // 10-23-24
    // Multiplies two given (rational) matrices together. If the matrices 
    // are incompatible, this returns the "error matrix"

    // check compatibility
    if (a.cols != b.rows) { return matrix_init_error(); }

    // if a is (NxM) and b is (MxP), the resulting matrix is (NxP)
    struct rat_matrix result = rat_matrix_init(a.rows, b.cols);

    for (int row = 0; row < result.rows; row++)
    {

        for (int col = 0; col < result.cols; col++)
        {
            
            // calculate dot product
            struct fraction current = fraction_init(0, 1, 1);

            for (int x = 0; x < a.cols; x++)
            {

                struct fraction a_elt = a.elements[rowcol_to_index(row, x)];
                struct fraction b_elt = b.elements[rowcol_to_index(x, col)];

                current = fraction_add(current, fraction_mul(a_elt, b_elt));

            }

            result.elements[rowcol_to_index(row, col)] = current;

        }


    }

    return result;

}

int8 matrix_fill(struct rat_matrix* mat, struct fraction* nums)
{

    u64 num_index = 0;
    for (int x = 0; x < mat->rows; x++)
    {

        for (int y = 0; y < mat->cols; y++)
        {

            mat->elements[rowcol_to_index(x, y)] = nums[num_index];
            num_index += 1;
        }
    }

    return 0;

}

struct rat_matrix matrix_init_error()
{
    struct rat_matrix a;

    a.rows = 1;
    a.cols = 1;

    a.elements[0] = fraction_init_error();

    return a;
}

bool matrix_is_error(struct rat_matrix a)
{
    if (a.rows != 1) { return false; }
    if (a.cols != 1) { return false; }

    if (!fraction_is_error(a.elements[0])) { return false; }

    return true;
}

u64 rowcol_to_index(u8 row, u8 col)
{
    return (row * RAT_MATR_MAX + col);
}
