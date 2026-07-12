#ifndef DRAWMATRIX_H
#define DRAWMATRIX_H
#include "raylib.h"

#define MAX_DIM 5

typedef struct { 
    int rows, cols; 
    float v[MAX_DIM][MAX_DIM]; 
} MatrixData;

void drawMatrix(const MatrixData *m, int x, int y, const char *name);



#endif