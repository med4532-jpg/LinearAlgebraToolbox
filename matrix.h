
#ifndef MATRIX_H
#define MATRIX_H

typedef struct
{
    int rows;
    int cols;
    double **data;
} MathMatrix;

/*
 * Erstellt eine Matrix mit dynamischer Speicherverwaltung.
 *
 * Rückgabe:
 *   Matrix mit rows Zeilen und cols Spalten.
 *   Bei ungültigen Dimensionen oder Speicherfehler wird eine leere Matrix zurückgegeben.
 */
MathMatrix createMatrix(int rows, int cols);

/*
 * Gibt den dynamisch belegten Speicher einer Matrix frei.
 */
void freeMatrix(MathMatrix *matrix);

/*
 * Addiert zwei Matrizen gleicher Dimension.
 */
MathMatrix addMatrices(MathMatrix a, MathMatrix b);

/*
 * Multipliziert zwei Matrizen, falls die Dimensionen kompatibel sind.
 */
MathMatrix multiplyMatrices(MathMatrix a, MathMatrix b);

/*
 * Erstellt die transponierte Matrix.
 */
MathMatrix transposeMatrix(MathMatrix matrix);

#endif


