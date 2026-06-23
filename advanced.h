#ifndef ADVANCED_H
#define ADVANCED_H

#include "matrix.h"

double determinant2x2(Matrix matrix);

int rank2x2(Matrix matrix);

int inverse2x2(Matrix matrix, Matrix *inverse);

void eigenvalues2x2(Matrix matrix,
                    double *lambda1,
                    double *lambda2);

#endif 