#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix createMatrix(int rows, int cols);
void freeMatrix(Matrix *matrix);

Matrix addMatrices(Matrix a, Matrix b);
Matrix multiplyMatrices(Matrix a, Matrix b);
Matrix transposeMatrix(Matrix matrix); 

#endif