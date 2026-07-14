#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "drawMatrix.h"
#include "ui_components.h"

#include "matrix.h"
#include "advanced.h"
#include "gauss.h"

#define TAB_COUNT 6



//Unsere Tabs für die verschiedenen Anwendungen
typedef enum {CALC, DET, INV, RANK, EIG, GAUSS} Tab;

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

// Hilfsfunktion: Setzt alle Werte einer Matrix auf 0 zurück
void clearMatrixData(MatrixData *matrix) {
    for (int r = 0; r < MAX_DIM; r++) {
        for (int c = 0; c < MAX_DIM; c++) {
            matrix->v[r][c] = 0.0f;
        }
    }
}

static void writeMatrixToFile(FILE *f, const char *label, const MatrixData *m) {
    fprintf(f, "# %s\n", label);
    fprintf(f, "%d %d\n", m->rows, m->cols);
    for (int r = 0; r < m->rows; r++) {
        for (int c = 0; c < m->cols; c++) {
            fprintf(f, "%g ", (double)m->v[r][c]);
        }
        fprintf(f, "\n");
    }
}


static bool readMatrixFromFile(FILE *f, MatrixData *m) {
    char line[256];
 
    do {
        if (!fgets(line, sizeof(line), f)) return false;
    } while (line[0] == '#' || line[0] == '\n' || line[0] == '\r');
 
    int rows, cols;
    if (sscanf(line, "%d %d", &rows, &cols) != 2) return false;
 
    if (rows < 1) rows = 1;
    if (rows > MAX_DIM) rows = MAX_DIM;
    if (cols < 1) cols = 1;
    if (cols > MAX_DIM) cols = MAX_DIM;
 
    m->rows = rows;
    m->cols = cols;
 
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (fscanf(f, "%f", &m->v[r][c]) != 1) return false;
        }
    }
    // Rest der letzten Zeilenumbrüche konsumieren, bevor die nächste Matrix gelesen wird
    int ch;
    while ((ch = fgetc(f)) != '\n' && ch != EOF) { }
 
    return true;
}


static void baueDateiname(char *out, size_t outSize, const char *tabName) {
    snprintf(out, outSize, "daten_%s.txt", tabName);
}


static bool speichereDaten(const char *dateiname, Tab tab, MatrixData tabMatrizen[], MatrixData *matrixCalcB, MatrixData *matrixGaussB, MatrixData *matrixCalcResult, bool zeigeErgebnisMatrix, const char *ergebnisText) {
    FILE *f = fopen(dateiname, "w");
    if (!f) return false;
 
    if (tab == CALC) {
        writeMatrixToFile(f, "Matrix A", &tabMatrizen[CALC]);
        writeMatrixToFile(f, "Matrix B", matrixCalcB);
    } else if (tab == GAUSS) {
        writeMatrixToFile(f, "Koeffizientenmatrix A", &tabMatrizen[GAUSS]);
        writeMatrixToFile(f, "Vektor b", matrixGaussB);
    } else {
        writeMatrixToFile(f, "Matrix", &tabMatrizen[tab]);
    }
 
    if (zeigeErgebnisMatrix) {
        writeMatrixToFile(f, "Ergebnis-Matrix", matrixCalcResult);
    }
    if (ergebnisText != NULL && ergebnisText[0] != '\0') {
        fprintf(f, "# Ergebnistext\n%s\n", ergebnisText);
    }
 
    fclose(f);
    return true;
}
static bool ladeDaten(const char *dateiname, Tab tab, MatrixData tabMatrizen[], MatrixData *matrixCalcB, MatrixData *matrixGaussB) {
    FILE *f = fopen(dateiname, "r");
    if (!f) return false;
 
    bool ok = true;
    if (tab == CALC) {
        ok = readMatrixFromFile(f, &tabMatrizen[CALC]) && ok;
        ok = readMatrixFromFile(f, matrixCalcB) && ok;
    } else if (tab == GAUSS) {
        ok = readMatrixFromFile(f, &tabMatrizen[GAUSS]) && ok;
        ok = readMatrixFromFile(f, matrixGaussB) && ok;
    } else {
        ok = readMatrixFromFile(f, &tabMatrizen[tab]) && ok;
    }
 
    fclose(f);
    return ok;
}




int main(void){

    InitWindow(1120,1100,"Linear Algebra ToolBox");
    SetTargetFPS(60);

    MatrixData tabMatrizen[TAB_COUNT] = {
        {3, 3, {{0}}}, // CALC
        {2, 2, {{0}}}, // DET
        {2, 2, {{0}}}, // INV
        {2, 2, {{0}}}, // RANK
        {2, 2, {{0}}},  // EIG
        {3, 3, {{0}}}  // GAUSS
    };

    bool zeigeErgebnisMatrix = false;

    char dateiStatus[160] = "";

    // Puffer für Textausgaben (z.B. Determinante = 5)
    char ergebnisText[128] = "";
    //Die zweite Matrix für den Rechnen-Tab (Matrix B)
    MatrixData matrixCalcB = {3, 3, {{0}}};
    MatrixData matrixGaussB = {3,1,{{0}}}; //3x1 Spaltenvektor
    MatrixData matrixCalcResult = {3, 3, {{0}}};

    //Wir starten beim Register "Rechnen"
    Tab aktuellerTab = CALC;
    MatrixOp aktiveOperation = OP_NONE;
    const char *tabNamen[TAB_COUNT] = {"Rechnen", "Determinante", "Inverse", "Rang", "Eigenwerte", "Gauss"};

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
            } 
            else if (aktuellerTab == GAUSS) {
                //Im Gauß-Tab prüfen wir Koeffizientenmatrix A und Vektor b
                handleMatrixClick(&tabMatrizen[GAUSS], 50, 200, mouse);
                handleMatrixClick(&matrixGaussB, 580, 200, mouse);
            }else {
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
      
            
            if (drawButton((Rectangle){800, 450, 170, 40}, "Transponieren (T)", PURPLE, false, mouse)) {
                
                //Matrix A aus dem UI holen, transponieren und wieder zurückschreiben
                MathMatrix mA = UItoMath(&tabMatrizen[CALC]);
                MathMatrix mATrans = transposeMatrix(mA); // Ruft deine Funktion aus matrix.c auf!
                MathToUI(mATrans, &tabMatrizen[CALC]);
                
                //Matrix B aus dem UI holen, transponieren und wieder zurückschreiben
                MathMatrix mB = UItoMath(&matrixCalcB);
                MathMatrix mBTrans = transposeMatrix(mB); // Ruft deine Funktion aus matrix.c auf!
                MathToUI(mBTrans, &matrixCalcB);
                
                //Dynamisch belegten Speicher der temporären Matrizen sauber freigeben
                freeMatrix(&mA);
                freeMatrix(&mATrans);
                freeMatrix(&mB);
                freeMatrix(&mBTrans);
                
                //Ein eventuell noch angezeigtes altes Rechenergebnis ausblenden
                zeigeErgebnisMatrix = false;
                ergebnisText[0] = '\0';
            }


            if (drawButton((Rectangle){250, 450, 150, 40}, "Addieren (+)", BLUE, (aktiveOperation == OP_ADD), mouse)) {
                aktiveOperation = OP_ADD;
                MathMatrix mA = UItoMath(&tabMatrizen[CALC]);
                MathMatrix mB = UItoMath(&matrixCalcB);
                
                MathMatrix mRes = addMatrices(mA, mB); // Deine Funktion aus matrix.h (Ori & Nik)!
                
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
                
                MathMatrix mRes = multiplyMatrices(mA, mB); // Deine Funktion aus matrix.h! (Ori & Nik)
                
                MathToUI(mRes, &matrixCalcResult);
                zeigeErgebnisMatrix = true;
                ergebnisText[0] = '\0';
                
                freeMatrix(&mA); freeMatrix(&mB); freeMatrix(&mRes);
            }

            
                
            // Ergebnismatrix unter den Knöpfen anzeigen (editable = false -> schreibgeschützt)
            if (zeigeErgebnisMatrix) {
                drawMatrix(&matrixCalcResult, 50, 540, "Ergebnis-Matrix", false);
            }
             

        } else {
            if (aktuellerTab == GAUSS) {
                drawMatrix(&tabMatrizen[GAUSS], 50, 200, "Koeffizientenmatrix A", true);
                drawMatrix(&matrixGaussB, 580, 200, "Vektor b", true);
            } else{
            // In allen anderen Tabs generieren wir den Standardtitel und zeichnen eine Matrix
            char matrixTitel[64];
            snprintf(matrixTitel, sizeof(matrixTitel), "Matrix für %s (Klicke Zelle zum Editieren)", tabNamen[aktuellerTab]);
            drawMatrix(&tabMatrizen[aktuellerTab], 50, 200, matrixTitel, true);
            }        
            
            
            
        }
        if(aktuellerTab == DET){
            if (drawButton((Rectangle){50, 400, 260, 40}, "Determinante berechnen", DARKGRAY, false, mouse)) {
                // Hier die jeweilige Berechnung (Det, Inv, Rank, Eig) triggern!
                printf("%s wird berechnet...\n", tabNamen[aktuellerTab]);
    
                MathMatrix m = UItoMath(&tabMatrizen[DET]);
                
                // 2. Funktion aus advanced.h berechnen
                double det = determinant2x2(m);
                
                // 3. Ergebnis in unseren Textpuffer schreiben
                snprintf(ergebnisText, sizeof(ergebnisText), "Determinante = %g", det);
                
                // 4. Dynamischen Speicher der MathMatrix wieder freigeben
                freeMatrix(&m);   
            }    
        }
        

        if (aktuellerTab == INV) {
            if (drawButton((Rectangle){50, 400, 260, 40}, "Inverse berechnen", DARKGRAY, false, mouse)) {
                
                // 1. Eingabematrix in mathematisches Format konvertieren
                MathMatrix m = UItoMath(&tabMatrizen[INV]);
                MathMatrix mInv; // Hier speichert deine Funktion das Ergebnis hinein
                
                
                if (inverse2x2(m, &mInv)) {
                    // Berechnung erfolgreich (Det != 0)
                    MathToUI(mInv, &matrixCalcResult);
                    zeigeErgebnisMatrix = true;
                    snprintf(ergebnisText, sizeof(ergebnisText), "Inverse erfolgreich berechnet!");
                    freeMatrix(&mInv); // Speicher der erzeugten Ergebnis-Matrix freigeben
                } else {
                    // Fehlerfall (z. B. Determinante ist 0)
                    zeigeErgebnisMatrix = false;
                    snprintf(ergebnisText, sizeof(ergebnisText), "Fehler: Matrix ist singulär (Det = 0) -> Keine Inverse!");
                }
                
                freeMatrix(&m); // Speicher der Eingabematrix freigeben
            }
        }
        //Verbindung zwischen RANK und Button
        if (aktuellerTab == RANK) {
            if (drawButton((Rectangle){50, 400, 260, 40}, "Rang berechnen", DARKGRAY, false, mouse)) {               
                MathMatrix m = UItoMath(&tabMatrizen[RANK]);
                int rang = rank2x2(m);              
                snprintf(ergebnisText, sizeof(ergebnisText), "Rang der Matrix = %d", rang);
                freeMatrix(&m);
            }
        }

        if (aktuellerTab == EIG) {
            if (drawButton((Rectangle){50, 400, 260, 40}, "Eigenwerte berechnen", DARKGRAY, false, mouse)) {              
                MathMatrix m = UItoMath(&tabMatrizen[EIG]);              
                double l1 = 0.0;
                double l2 = 0.0;              
                eigenvalues2x2(m, &l1, &l2);               
                //Prüfen, ob die Eigenwerte komplex sind (werden als NAN zurückgegeben)
                if (isnan(l1) || isnan(l2)) {
                    snprintf(ergebnisText, sizeof(ergebnisText), "Eigenwerte: Keine reellen Eigenwerte vorhanden (komplex).");
                } else {
                    snprintf(ergebnisText, sizeof(ergebnisText), "Eigenwerte: lambda_1 = %g, lambda_2 = %g", l1, l2);
                }
                freeMatrix(&m);
            }
        }

        if (aktuellerTab == GAUSS) {
            if (drawButton((Rectangle){50, 450, 260, 40}, "LGS lösen (Gauß)", DARKGRAY, false, mouse)) {
                
                // 1. Matrizen in das mathematische Format konvertieren
                MathMatrix mA = UItoMath(&tabMatrizen[GAUSS]);
                MathMatrix mB = UItoMath(&matrixGaussB); 
                MathMatrix mSol;
                
                // 2. lösen
                if (solveGauss(mA, mB, &mSol)) {
                    MathToUI(mSol, &matrixCalcResult);
                    zeigeErgebnisMatrix = true;
                    snprintf(ergebnisText, sizeof(ergebnisText), "LGS erfolgreich gelöst!");
                    freeMatrix(&mSol);
                } else {
                    zeigeErgebnisMatrix = false;
                    snprintf(ergebnisText, sizeof(ergebnisText), "Fehler: System nicht lösbar oder Dimensionen inkompatibel!");
                }
                freeMatrix(&mA); freeMatrix(&mB);
            }
        }

        // Ausgabe des Ergebnis
        if (ergebnisText[0] != '\0') {
            DrawText(ergebnisText, 50, 500, 24, MAROON);
        }

        if (aktuellerTab == INV && zeigeErgebnisMatrix) {
            drawMatrix(&matrixCalcResult, 50, 510, "Invertierte Matrix A^-1", false);
        }

       
        if (aktuellerTab == GAUSS && zeigeErgebnisMatrix) {
            drawMatrix(&matrixCalcResult, 50, 530, " ", false);
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
        if (aktuellerTab == GAUSS) DrawText("Modus: Gauß-Verfahren", 35, 130, 20, DARKGRAY);
        

        if (drawButton((Rectangle){920, 125, 150, 35}, "Matrix leeren", MAROON, false, mouse)) {
            // Je nachdem welcher Tab aktiv ist leeren wir die dazugehörigen Matrizen
            if (aktuellerTab == CALC) {
                clearMatrixData(&tabMatrizen[CALC]);
                clearMatrixData(&matrixCalcB);
                clearMatrixData(&matrixCalcResult);
            } else if (aktuellerTab == GAUSS) {
                clearMatrixData(&tabMatrizen[GAUSS]);
                clearMatrixData(&matrixGaussB);
                clearMatrixData(&matrixCalcResult);
            } else {
                clearMatrixData(&tabMatrizen[aktuellerTab]);
            }
            
            // Nach dem Leeren setzen wir das berechnete Ergebnis zurück
            zeigeErgebnisMatrix = false;
            ergebnisText[0] = '\0';
            dateiStatus[0] = '\0';   // wird beim Leeren mit zurückgesetzt
        }

        if (drawButton((Rectangle){920, 170, 150, 35}, "Öffnen", DARKGREEN, false, mouse)) {
            char dateiname[64];
            baueDateiname(dateiname, sizeof(dateiname), tabNamen[aktuellerTab]);
 
            if (ladeDaten(dateiname, aktuellerTab, tabMatrizen, &matrixCalcB, &matrixGaussB)) {
                snprintf(dateiStatus, sizeof(dateiStatus), "Geladen aus %s", dateiname);
                // Nach dem Laden ein evtl. altes Ergebnis ausblenden, da es nicht mehr zu den neuen Werten passt
                zeigeErgebnisMatrix = false;
                ergebnisText[0] = '\0';
            } else {
                snprintf(dateiStatus, sizeof(dateiStatus), "Fehler: %s konnte nicht gelesen werden!", dateiname);
            }
        }
 
        // "Speichern"-Button: schreibt Eingabematrizen + aktuelles Ergebnis des Tabs in eine Datei
        if (drawButton((Rectangle){920, 215, 150, 35}, "Speichern", DARKGREEN, false, mouse)) {
            char dateiname[64];
            baueDateiname(dateiname, sizeof(dateiname), tabNamen[aktuellerTab]);
 
            if (speichereDaten(dateiname, aktuellerTab, tabMatrizen, &matrixCalcB, &matrixGaussB, &matrixCalcResult, zeigeErgebnisMatrix, ergebnisText)) {
                snprintf(dateiStatus, sizeof(dateiStatus), "Gespeichert in %s", dateiname);
            } else {
                snprintf(dateiStatus, sizeof(dateiStatus), "Fehler: %s konnte nicht geschrieben werden!", dateiname);
            }
        }
 
        // Statusmeldung zu Öffnen/Speichern anzeigen
        if (dateiStatus[0] != '\0') {
            DrawText(dateiStatus, 920, 255, 16, DARKGRAY);
        }

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
