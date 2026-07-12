#ifndef DRAWMATRIX_H
#define DRAWMATRIX_H
#include "raylib.h"

#define MAX_DIM 5

typedef struct { 
    int rows, cols; 
    float v[MAX_DIM][MAX_DIM]; 
} MatrixData;

extern MatrixData *activeEditMatrix; // Zeiger auf die Matrix, die gerade editiert wird
extern int editRow;                  // Aktive Zeile (-1 bedeutet keine)
extern int editCol;                  // Aktive Spalte (-1 bedeutet keine)
extern char editBuffer[24];          // Textpuffer für die getippten Zahlen


void drawMatrix(const MatrixData *m, int x, int y, const char *name,bool editable);
void handleMatrixClick(MatrixData *m, int x, int y, Vector2 mousePos);
void handleKeyboardInput(void);


#endif