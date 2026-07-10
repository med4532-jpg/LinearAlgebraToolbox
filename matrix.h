#ifndef MATRIX_H
#define MATRIX_H

typedef struct
{
    int rows;
    int cols;
    double **data;
} Matrix;

/*
 * Erstellt eine Matrix mit dynamischer Speicherverwaltung.
 *
 * Rückgabe:
 *   Matrix mit rows Zeilen und cols Spalten.
 *   Bei ungültigen Dimensionen oder Speicherfehler wird eine leere Matrix zurückgegeben.
 */
Matrix createMatrix(int rows, int cols);

/*
 * Gibt den dynamisch belegten Speicher einer Matrix frei.
 */
void freeMatrix(Matrix *matrix);

/*
 * Addiert zwei Matrizen gleicher Dimension.
 */
Matrix addMatrices(Matrix a, Matrix b);

/*
 * Multipliziert zwei Matrizen, falls die Dimensionen kompatibel sind.
 */
Matrix multiplyMatrices(Matrix a, Matrix b);

/*
 * Erstellt die transponierte Matrix.
 */
Matrix transposeMatrix(Matrix matrix);

#endif