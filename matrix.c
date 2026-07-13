#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

/*
 * Erstellt eine leere Matrix.
 */
static MathMatrix createEmptyMatrix(void)
{
    MathMatrix matrix;
    matrix.rows = 0;
    matrix.cols = 0;
    matrix.data = NULL;
    return matrix;
}

/*
 * Erstellt eine Matrix mit dynamischer Speicherverwaltung.
 */
MathMatrix createMatrix(int rows, int cols)
{
    MathMatrix matrix = createEmptyMatrix();

    if (rows <= 0 || cols <= 0) {
        return matrix;
    }

    matrix.rows = rows;
    matrix.cols = cols;

    /* Speicher für Zeiger auf die Matrixzeilen reservieren */
    matrix.data = malloc(rows * sizeof(double *));

    if (matrix.data == NULL) {
        return createEmptyMatrix();
    }

    /* Speicher für jede Zeile reservieren */
    for (int i = 0; i < rows; i++) {
        matrix.data[i] = calloc(cols, sizeof(double));

        if (matrix.data[i] == NULL) {

            /* Bereits reservierten Speicher freigeben */
            for (int j = 0; j < i; j++) {
                free(matrix.data[j]);
            }

            free(matrix.data);
            return createEmptyMatrix();
        }
    }

    return matrix;
}

/*
 * Gibt den belegten Speicher einer Matrix frei.
 */
void freeMatrix(MathMatrix *matrix)
{
    if (matrix == NULL || matrix->data == NULL) {
        return;
    }

    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }

    free(matrix->data);

    matrix->rows = 0;
    matrix->cols = 0;
    matrix->data = NULL;
}

/*
 * Addiert zwei Matrizen gleicher Dimension.
 */
MathMatrix addMatrices(MathMatrix a, MathMatrix b)
{
    if (a.data == NULL || b.data == NULL ||
        a.rows != b.rows || a.cols != b.cols) {
        return createEmptyMatrix();
    }

    MathMatrix result = createMatrix(a.rows, a.cols);

    if (result.data == NULL) {
        return result;
    }

    /* Elemente beider Matrizen addieren */
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }

    return result;
}

/*
 * Multipliziert zwei Matrizen.
 */
MathMatrix multiplyMatrices(MathMatrix a, MathMatrix b)
{
    if (a.data == NULL || b.data == NULL || a.cols != b.rows) {
        return createEmptyMatrix();
    }

    MathMatrix result = createMatrix(a.rows, b.cols);

    if (result.data == NULL) {
        return result;
    }

    /* Matrixmultiplikation durchführen */
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            for (int k = 0; k < a.cols; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }

    return result;
}

/*
 * Erstellt die transponierte Matrix.
 */
MathMatrix transposeMatrix(MathMatrix matrix)
{
    if (matrix.data == NULL) {
        return createEmptyMatrix();
    }

    MathMatrix result = createMatrix(matrix.cols, matrix.rows);

    if (result.data == NULL) {
        return result;
    }

    /* Zeilen und Spalten vertauschen */
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            result.data[j][i] = matrix.data[i][j];
        }
    }

    return result;
}
