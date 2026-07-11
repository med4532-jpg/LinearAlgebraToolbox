#ifndef GAUSS_H
#define GAUSS_H

#include "matrix.h"

/*
 * Löst ein lineares Gleichungssystem mit dem Gauß-Verfahren.
 *
 * Rückgabe:
 *   1 bei erfolgreicher Berechnung,
 *   0 falls das Gleichungssystem nicht lösbar ist oder die Eingabe ungültig ist.
 */
int solveGauss(Matrix A, Matrix b, Matrix *solution);

#endif