#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "matrix.h"
#include "advanced.h"

static int almostEqual(double a, double b)
{
    return fabs(a - b) < 1e-6;
}

static void testRegularMatrix(void)
{
    Matrix matrix = createMatrix(2, 2);
    Matrix inverse = {0};

    matrix.data[0][0] = 1.0;
    matrix.data[0][1] = 2.0;
    matrix.data[1][0] = 3.0;
    matrix.data[1][1] = 4.0;

    double lambda1;
    double lambda2;

    assert(almostEqual(determinant2x2(matrix), -2.0));
    assert(rank2x2(matrix) == 2);
    assert(inverse2x2(matrix, &inverse) == 1);

    assert(almostEqual(inverse.data[0][0], -2.0));
    assert(almostEqual(inverse.data[0][1], 1.0));
    assert(almostEqual(inverse.data[1][0], 1.5));
    assert(almostEqual(inverse.data[1][1], -0.5));

    eigenvalues2x2(matrix, &lambda1, &lambda2);

    assert(almostEqual(lambda1, 5.372281323));
    assert(almostEqual(lambda2, -0.372281323));

    freeMatrix(&inverse);
    freeMatrix(&matrix);

    printf("Test 1 bestanden: reguläre Matrix\n");
}

static void testSingularMatrix(void)
{
    Matrix matrix = createMatrix(2, 2);
    Matrix inverse = {0};

    matrix.data[0][0] = 1.0;
    matrix.data[0][1] = 2.0;
    matrix.data[1][0] = 2.0;
    matrix.data[1][1] = 4.0;

    double lambda1;
    double lambda2;

    assert(almostEqual(determinant2x2(matrix), 0.0));
    assert(rank2x2(matrix) == 1);
    assert(inverse2x2(matrix, &inverse) == 0);

    eigenvalues2x2(matrix, &lambda1, &lambda2);

    assert(almostEqual(lambda1, 5.0));
    assert(almostEqual(lambda2, 0.0));

    freeMatrix(&matrix);

    printf("Test 2 bestanden: singuläre Matrix\n");
}

static void testZeroMatrix(void)
{
    Matrix matrix = createMatrix(2, 2);
    Matrix inverse = {0};

    double lambda1;
    double lambda2;

    assert(almostEqual(determinant2x2(matrix), 0.0));
    assert(rank2x2(matrix) == 0);
    assert(inverse2x2(matrix, &inverse) == 0);

    eigenvalues2x2(matrix, &lambda1, &lambda2);

    assert(almostEqual(lambda1, 0.0));
    assert(almostEqual(lambda2, 0.0));

    freeMatrix(&matrix);

    printf("Test 3 bestanden: Nullmatrix\n");
}

static void testComplexEigenvalues(void)
{
    Matrix matrix = createMatrix(2, 2);
    Matrix inverse = {0};

    matrix.data[0][0] = 0.0;
    matrix.data[0][1] = -1.0;
    matrix.data[1][0] = 1.0;
    matrix.data[1][1] = 0.0;

    double lambda1;
    double lambda2;

    assert(almostEqual(determinant2x2(matrix), 1.0));
    assert(rank2x2(matrix) == 2);
    assert(inverse2x2(matrix, &inverse) == 1);

    eigenvalues2x2(matrix, &lambda1, &lambda2);

    assert(isnan(lambda1));
    assert(isnan(lambda2));

    freeMatrix(&inverse);
    freeMatrix(&matrix);

    printf("Test 4 bestanden: komplexe Eigenwerte\n");
}

int main(void)
{
    testRegularMatrix();
    testSingularMatrix();
    testZeroMatrix();
    testComplexEigenvalues();

    printf("Alle Tests erfolgreich bestanden.\n");

    return 0;
}