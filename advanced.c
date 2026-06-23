#include <stddef.h>
#include <math.h>
#include "advanced.h"

double determinant2x2(Matrix matrix)
{
    if (matrix.rows != 2 || matrix.cols != 2 || matrix.data == NULL)
    {
        return 0.0;
    }

    return matrix.data[0][0] * matrix.data[1][1]
         - matrix.data[0][1] * matrix.data[1][0];
}

int rank2x2(Matrix matrix)
{
    if (matrix.rows != 2 || matrix.cols != 2 || matrix.data == NULL)
    {
        return 0;
    }

    double det = determinant2x2(matrix);

    if (fabs(det) > 1e-9)
    {
        return 2;
    }

    if (matrix.data[0][0] != 0.0 ||
        matrix.data[0][1] != 0.0 ||
        matrix.data[1][0] != 0.0 ||
        matrix.data[1][1] != 0.0)
    {
        return 1;
    }

    return 0;
}

int inverse2x2(Matrix matrix, Matrix *inverse)
{
    if (matrix.rows != 2 || matrix.cols != 2 ||
        matrix.data == NULL || inverse == NULL)
    {
        return 0;
    }

    double det = determinant2x2(matrix);

    if (fabs(det) < 1e-9)
    {
        return 0;
    }

    *inverse = createMatrix(2, 2);

    if (inverse->rows != 2 ||
        inverse->cols != 2 ||
        inverse->data == NULL)
    {
        return 0;
    }

    inverse->data[0][0] =  matrix.data[1][1] / det;
    inverse->data[0][1] = -matrix.data[0][1] / det;
    inverse->data[1][0] = -matrix.data[1][0] / det;
    inverse->data[1][1] =  matrix.data[0][0] / det;

    return 1;
}

void eigenvalues2x2(Matrix matrix, double *lambda1, double *lambda2)
{
    if (matrix.rows != 2 || matrix.cols != 2 ||
        matrix.data == NULL ||
        lambda1 == NULL ||
        lambda2 == NULL)
    {
        return;
    }

    double a = matrix.data[0][0];
    double b = matrix.data[0][1];
    double c = matrix.data[1][0];
    double d = matrix.data[1][1];

    double trace = a + d;
    double det = determinant2x2(matrix);
    double discriminant = trace * trace - 4.0 * det;

    if (discriminant < 0.0)
    {
        *lambda1 = NAN;
        *lambda2 = NAN;
        return;
    }

    *lambda1 = (trace + sqrt(discriminant)) / 2.0;
    *lambda2 = (trace - sqrt(discriminant)) / 2.0;
}