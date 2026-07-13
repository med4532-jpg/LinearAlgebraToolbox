#include <stddef.h>
#include <math.h>
#include "advanced.h"

/*
 * Berechnet die Determinante einer 2×2-Matrix.
 */
double determinant2x2(MathMatrix matrix)
{
    if (matrix.rows != 2 || matrix.cols != 2 || matrix.data == NULL)
    {
        return 0.0;
    }

    return matrix.data[0][0] * matrix.data[1][1]
         - matrix.data[0][1] * matrix.data[1][0];
}

/*
 * Bestimmt den Rang einer 2×2-Matrix.
 */
int rank2x2(MathMatrix matrix)
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

/*
 * Berechnet die Inverse einer 2×2-Matrix.
 *
 * Rückgabe:
 *   1 bei erfolgreicher Berechnung,
 *   0 falls keine Inverse existiert oder die Eingabe ungültig ist.
 */
int inverse2x2(MathMatrix matrix, MathMatrix *inverse)
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

    /* Ergebnismatrix erzeugen */
    *inverse = createMatrix(2, 2);

    if (inverse->rows != 2 ||
        inverse->cols != 2 ||
        inverse->data == NULL)
    {
        return 0;
    }

    /* Einträge der Inversen berechnen */
    inverse->data[0][0] =  matrix.data[1][1] / det;
    inverse->data[0][1] = -matrix.data[0][1] / det;
    inverse->data[1][0] = -matrix.data[1][0] / det;
    inverse->data[1][1] =  matrix.data[0][0] / det;

    return 1;
}

/*
 * Berechnet die Eigenwerte einer 2×2-Matrix.
 *
 * Komplexe Eigenwerte werden durch NAN dargestellt.
 */
void eigenvalues2x2(MathMatrix matrix, double *lambda1, double *lambda2)
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

    /* Spur, Determinante und Diskriminante berechnen */
    double trace = a + d;
    double det = determinant2x2(matrix);
    double discriminant = trace * trace - 4.0 * det;

    if (discriminant < 0.0)
    {
        *lambda1 = NAN;
        *lambda2 = NAN;
        return;
    }

    /* Eigenwerte berechnen */
    *lambda1 = (trace + sqrt(discriminant)) / 2.0;
    *lambda2 = (trace - sqrt(discriminant)) / 2.0;
}