#include <math.h>
#include <stdlib.h>
#include "gauss.h"

int solveGauss(Matrix A, Matrix b, Matrix *solution)
{
    if (A.data == NULL || b.data == NULL || solution == NULL) {
        return 0;
    }

    if (A.rows != A.cols || b.rows != A.rows || b.cols != 1) {
        return 0;
    }

    int n = A.rows;

    Matrix matrix = createMatrix(n, n);
    Matrix vector = createMatrix(n, 1);

    if (matrix.data == NULL || vector.data == NULL) {
        freeMatrix(&matrix);
        freeMatrix(&vector);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        vector.data[i][0] = b.data[i][0];

        for (int j = 0; j < n; j++) {
            matrix.data[i][j] = A.data[i][j];
        }
    }

    for (int i = 0; i < n; i++) {

        int pivot = i;

        for (int j = i + 1; j < n; j++) {
            if (fabs(matrix.data[j][i]) > fabs(matrix.data[pivot][i])) {
                pivot = j;
            }
        }

        if (fabs(matrix.data[pivot][i]) < 1e-9) {
            freeMatrix(&matrix);
            freeMatrix(&vector);
            return 0;
        }

        if (pivot != i) {

            double *tempRow = matrix.data[i];
            matrix.data[i] = matrix.data[pivot];
            matrix.data[pivot] = tempRow;

            double tempValue = vector.data[i][0];
            vector.data[i][0] = vector.data[pivot][0];
            vector.data[pivot][0] = tempValue;
        }

        for (int j = i + 1; j < n; j++) {

            double factor = matrix.data[j][i] / matrix.data[i][i];

            for (int k = i; k < n; k++) {
                matrix.data[j][k] -= factor * matrix.data[i][k];
            }

            vector.data[j][0] -= factor * vector.data[i][0];
        }
    }

    *solution = createMatrix(n, 1);

    if (solution->data == NULL) {
        freeMatrix(&matrix);
        freeMatrix(&vector);
        return 0;
    }

    for (int i = n - 1; i >= 0; i--) {

        double sum = vector.data[i][0];

        for (int j = i + 1; j < n; j++) {
            sum -= matrix.data[i][j] * solution->data[j][0];
        }

        solution->data[i][0] = sum / matrix.data[i][i];
    }

    freeMatrix(&matrix);
    freeMatrix(&vector);

    return 1;
}