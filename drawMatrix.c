#include "drawMatrix.h"
#include <stdio.h> 

void drawMatrix(const MatrixData *m, int x, int y, const char *name) {
    int CELL = 58; // Größe einer Zelle
    int GAP = 7;   // Abstand zwischen Zellen
    
    DrawText(name, x, y - 30, 23, DARKBLUE); // Titel über der Matrix
    
    for (int r = 0; r < m->rows; r++) {
        for (int c = 0; c < m->cols; c++) {
            // Berechne die Position jeder einzelnen Zelle
            Rectangle cellRec = {(float)(x + c * (CELL + GAP)), (float)(y + r * (CELL + GAP)), CELL, CELL};
            
            // Zelle zeichnen
            DrawRectangleRounded(cellRec, 0.12f, 4, RAYWHITE);
            DrawRectangleRoundedLines(cellRec, 0.12f, 4, LIGHTGRAY);
            
            // Wert als Text in der Zelle anzeigen
            char textBuffer[16];
            snprintf(textBuffer, sizeof(textBuffer), "%g", m->v[r][c]);
            DrawText(textBuffer, (int)(cellRec.x + 15), (int)cellRec.y + 20, 17, DARKGRAY);
        }
    }
}