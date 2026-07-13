#ifndef ADVANCED_H
#define ADVANCED_H

#include "matrix.h"

/*
 * Berechnet die Determinante einer 2×2-Matrix.
 */
double determinant2x2(MathMatrix matrix);

/*
 * Bestimmt den Rang einer 2×2-Matrix.
 */
int rank2x2(MathMatrix matrix);

/*
 * Berechnet die Inverse einer 2×2-Matrix.
 *
 * Rückgabe:
 *   1 bei erfolgreicher Berechnung,
 *   0 falls keine Inverse existiert oder die Eingabe ungültig ist.
 */
int inverse2x2(MathMatrix matrix, MathMatrix *inverse);

/*
 * Berechnet die Eigenwerte einer 2×2-Matrix.
 *
 * Komplexe Eigenwerte werden durch NAN dargestellt.
 */
void eigenvalues2x2(MathMatrix matrix,
                    double *lambda1,
                    double *lambda2);

#endif