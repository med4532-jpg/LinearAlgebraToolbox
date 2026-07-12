#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "raylib.h"
#include <stdbool.h>

// Enum für die Rechenoperationen im ersten Tab
typedef enum { OP_NONE, OP_ADD, OP_SUB, OP_MUL,OP_GAUSS} MatrixOp;

// Zeichnet einen Button und gibt true zurück, wenn er geklickt wurde
bool drawButton(Rectangle rect, const char *text, Color baseColor, bool isActive, Vector2 mousePos);

#endif