#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "drawMatrix.h"
#include "ui_components.h"

#include "matrix.h"
#include "advanced.h"
#include "gauss.h"

#define TAB_COUNT 5



//Unsere Tabs für die verschiedenen Anwendungen
typedef enum {CALC, DET, INV, RANK, EIG} Tab;

// Konvertiert die UI-Matrix in die Mathe-Matrix
MathMatrix UItoMath(const MatrixData *src) {
    MathMatrix dest = createMatrix(src->rows, src->cols);
    if (dest.data != NULL) {
        for (int r = 0; r < src->rows; r++) {
            for (int c = 0; c < src->cols; c++) {
                dest.data[r][c] = (double)src->v[r][c];
            }
        }
    }
    return dest;
}

// Konvertiert die Mathe-Matrix zurück in die UI-Matrix
void MathToUI(MathMatrix src, MatrixData *dest) {
    dest->rows = src.rows;
    dest->cols = src.cols;
    for (int r = 0; r < src.rows && r < MAX_DIM; r++) {
        for (int c = 0; c < src.cols && c < MAX_DIM; c++) {
            dest->v[r][c] = (float)src.data[r][c];
        }
    }
}

int main(void){

    InitWindow(1120,1100,"Linear Algebra ToolBox");
    SetTargetFPS(60);

    MatrixData tabMatrizen[TAB_COUNT] = {
        {3, 3, {{0}}}, // CALC
        {3, 3, {{0}}}, // DET
        {3, 3, {{0}}}, // INV
        {3, 3, {{0}}}, // RANK
        {3, 3, {{0}}}  // EIG
    };

    bool zeigeErgebnisMatrix = false;

    // Puffer für Textausgaben (z.B. Determinante = 5)
    char ergebnisText[128] = "";
    //Die zweite Matrix für den Rechnen-Tab (Matrix B)
    MatrixData matrixCalcB = {3, 3, {{0}}};

    MatrixData matrixCalcResult = {3, 3, {{0}}};

    //Wir starten beim Register "Rechnen"
    Tab aktuellerTab = CALC;
    MatrixOp aktiveOperation = OP_NONE;
    const char *tabNamen[TAB_COUNT] = {"Rechnen", "Determinante", "Inverse", "Rang", "Eigenwerte"};

    while(!WindowShouldClose()){
        Vector2 mouse = GetMousePosition();
        
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            for(int i = 0; i< TAB_COUNT; i++){
                Rectangle r = {(float)(25+i*190), 65, 175, 38};
                if(CheckCollisionPointRec(mouse, r)){
                    aktuellerTab = (Tab)i;
                    activeEditMatrix = NULL;
                    editRow = editCol = -1;
                    zeigeErgebnisMatrix = false;
                    ergebnisText[0] = '\0';
                    aktiveOperation = OP_NONE;
                }
            }

            if (aktuellerTab == CALC) {
                // Im Rechnen-Tab prüfen wir BEIDE Matrizen (Matrix A links bei X: 50, Matrix B rechts bei X: 580)
                handleMatrixClick(&tabMatrizen[CALC], 50, 200, mouse);
                handleMatrixClick(&matrixCalcB, 580, 200, mouse);
            } else {
                // In allen anderen Tabs wird wie gewohnt nur die eine Standard-Matrix geklickt
                handleMatrixClick(&tabMatrizen[aktuellerTab], 50, 200, mouse);
            }
        }
        
        

        handleKeyboardInput();
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        ClearBackground((Color){243, 247, 252, 255});
        if (aktuellerTab == CALC) {
            // Im Rechnen-Tab zeichnen wir zwei Matrizen nebeneinander
            drawMatrix(&tabMatrizen[CALC], 50, 200, "Matrix A", true);
            drawMatrix(&matrixCalcB, 580, 200, "Matrix B", true);

            
            if (drawButton((Rectangle){250, 450, 150, 40}, "Addieren (+)", BLUE, (aktiveOperation == OP_ADD), mouse)) {
                aktiveOperation = OP_ADD;
                MathMatrix mA = UItoMath(&tabMatrizen[CALC]);
                MathMatrix mB = UItoMath(&matrixCalcB);
                
                MathMatrix mRes = addMatrices(mA, mB); // Deine Funktion aus matrix.h!
                
                MathToUI(mRes, &matrixCalcResult);
                zeigeErgebnisMatrix = true;
                ergebnisText[0] = '\0';
                
                freeMatrix(&mA); freeMatrix(&mB); freeMatrix(&mRes);
            }
            if (drawButton((Rectangle){420, 450, 170, 40}, "Subtrahieren (-)", ORANGE, (aktiveOperation == OP_SUB), mouse)) {
                aktiveOperation = OP_SUB;
                matrixCalcResult.rows = tabMatrizen[CALC].rows;
                matrixCalcResult.cols = tabMatrizen[CALC].cols;
                for(int r=0; r<matrixCalcResult.rows; r++) {
                    for(int c=0; c<matrixCalcResult.cols; c++) {
                        matrixCalcResult.v[r][c] = tabMatrizen[CALC].v[r][c] - matrixCalcB.v[r][c];
                    }
                }
                zeigeErgebnisMatrix = true;
                ergebnisText[0] = '\0';
            }
            if (drawButton((Rectangle){610, 450, 170, 40}, "Multiplizieren (x)", GREEN, (aktiveOperation == OP_MUL), mouse)) {
                aktiveOperation = OP_MUL;
                MathMatrix mA = UItoMath(&tabMatrizen[CALC]);
                MathMatrix mB = UItoMath(&matrixCalcB);
                
                MathMatrix mRes = multiplyMatrices(mA, mB); // Deine Funktion aus matrix.h!
                
                MathToUI(mRes, &matrixCalcResult);
                zeigeErgebnisMatrix = true;
                ergebnisText[0] = '\0';
                
                freeMatrix(&mA); freeMatrix(&mB); freeMatrix(&mRes);
            }

            if (drawButton((Rectangle){800, 450, 140, 40}, "Gauß", PURPLE, (aktiveOperation == OP_GAUSS), mouse)) {
                aktiveOperation = OP_GAUSS;
                MathMatrix mA = UItoMath(&tabMatrizen[CALC]);
                MathMatrix mB = UItoMath(&matrixCalcB); 
                MathMatrix mSol;
                
                // Deine Funktion aus gauss.h!
                if (solveGauss(mA, mB, &mSol)) {
                    MathToUI(mSol, &matrixCalcResult);
                    zeigeErgebnisMatrix = true;
                    snprintf(ergebnisText, sizeof(ergebnisText), "LGS erfolgreich gelöst!");
                    freeMatrix(&mSol);
                } else {
                    zeigeErgebnisMatrix = false;
                    snprintf(ergebnisText, sizeof(ergebnisText), "Fehler: System nicht lösbar oder Dimensionen inkompatibel (B muss Spaltenvektor sein!).");
                }
                freeMatrix(&mA); freeMatrix(&mB);
            }
                
            // Ergebnismatrix unter den Knöpfen anzeigen (editable = false -> schreibgeschützt)
            if (zeigeErgebnisMatrix) {
                drawMatrix(&matrixCalcResult, 50, 540, "Ergebnis-Matrix", false);
            }
             

        } else {
            // In allen anderen Tabs generieren wir den Standardtitel und zeichnen eine Matrix
            char matrixTitel[64];
            snprintf(matrixTitel, sizeof(matrixTitel), "Matrix für %s (Klicke Zelle zum Editieren)", tabNamen[aktuellerTab]);
            drawMatrix(&tabMatrizen[aktuellerTab], 50, 200, matrixTitel, true);

            char buttonText[64];
            snprintf(buttonText, sizeof(buttonText), "%s berechnen", tabNamen[aktuellerTab]);
            
            if (drawButton((Rectangle){50, 450, 240, 40}, buttonText, DARKGRAY, false, mouse)) {
                // Hier die jeweilige Berechnung (Det, Inv, Rank, Eig) triggern!
                printf("%s wird berechnet...\n", tabNamen[aktuellerTab]);
            }
        }

        // Tabs zeichnen
        for (int i = 0; i < TAB_COUNT; i++) {
            Rectangle r = {(float)(25 + i * 190), 65, 175, 38};
            Color farbe = (i == (int)aktuellerTab) ? DARKBLUE : GRAY;
            
            // Button zeichnen
            DrawRectangleRounded(r, 0.18f, 6, farbe);
            DrawText(tabNamen[i], (int)(r.x + 15), (int)r.y + 10, 18, RAYWHITE);
        }

        // Text je nach aktivem Tab anzeigen
        if (aktuellerTab == CALC) DrawText("Modus: Grundrechenarten", 35, 130, 20, DARKGRAY);
        if (aktuellerTab == DET) DrawText("Modus: Determinante", 35, 130, 20, DARKGRAY);
        if (aktuellerTab == INV) DrawText("Modus: Inverse", 35, 130, 20, DARKGRAY);
        if (aktuellerTab == RANK) DrawText("Modus: Rang", 35, 130, 20, DARKGRAY);
        if (aktuellerTab == EIG) DrawText("Modus: Eigenwerte", 35, 130, 20, DARKGRAY);
        
        EndDrawing();

    }

    CloseWindow();
    return 0;
}
