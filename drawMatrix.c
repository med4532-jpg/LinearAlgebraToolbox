#include "drawMatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

MatrixData *activeEditMatrix = NULL;
int editRow = -1;
int editCol = -1;
char editBuffer[24] = "";

// Hilfsfunktion: String in float umwandeln (Komma zu Punkt korrigieren)
static float stringToFloat(const char *s) {
    char temp[24];
    strncpy(temp, s, 23);
    temp[23] = 0;
    for (int i = 0; temp[i]; i++) {
        if (temp[i] == ',') temp[i] = '.'; // Erlaubt auch Komma-Eingabe
    }
    return strtof(temp, NULL);
}

// 1. Klick-Logik: Wurde eine Zelle angeklickt?
void handleMatrixClick(MatrixData *m, int x, int y, Vector2 mousePos) {
    int CELL = 58;
    int GAP = 7;

    for (int r = 0; r < m->rows; r++) {
        for (int c = 0; c < m->cols; c++) {
            Rectangle cellRec = {(float)(x + c * (CELL + GAP)), (float)(y + r * (CELL + GAP)), (float)CELL, (float)CELL};
            
            if (CheckCollisionPointRec(mousePos, cellRec)) {
                activeEditMatrix = m;
                editRow = r;
                editCol = c;
                // Aktuellen Wert der Zelle als Text in den Puffer laden
                snprintf(editBuffer, sizeof(editBuffer), "%g", m->v[r][c]);
                return;
            }
        }
    }
}

// 2. Tastatur-Logik: Zeichen abfangen
void handleKeyboardInput(void) {
    if (activeEditMatrix == NULL) return; // Keine Zelle aktiv -> nichts tun

    bool bufferChanged = false;

    // Gedrücktes Zeichen holen (0 wenn keins)
    int key = GetCharPressed();
    while (key > 0) {
        // Nur Zahlen, Minus, Punkt und Komma erlauben
        if (((key >= '0' && key <= '9') || key == '-' || key == '.' || key == ',') && strlen(editBuffer) < sizeof(editBuffer) - 1) {
            size_t len = strlen(editBuffer);
            editBuffer[len] = (char)key;
            editBuffer[len + 1] = '\0';
            bufferChanged = true;
        }
        key = GetCharPressed(); // Nächstes Zeichen in der Warteschlange prüfen
    }

    // WENN sich der Puffer geändert hat, schreiben wir den Wert SOFORT live in die Matrix
    if (bufferChanged) {
        // Falls der Puffer leer ist (z.B. alles gelöscht), setzen wir eine 0 an
        if (strlen(editBuffer) == 0 || (strlen(editBuffer) == 1 && editBuffer[0] == '-')) {
            activeEditMatrix->v[editRow][editCol] = 0.0f;
        } else {
            activeEditMatrix->v[editRow][editCol] = stringToFloat(editBuffer);
        }
    }

    

    // ESCAPE: Abbrechen ohne zu speichern
    if (IsKeyPressed(KEY_ESCAPE)) {
        activeEditMatrix = NULL;
        editRow = editCol = -1;
    }
}

void drawMatrix(const MatrixData *m, int x, int y, const char *name,bool editable) {
    int CELL = 58; // Größe einer Zelle
    int GAP = 7;   // Abstand zwischen Zellen
    
    DrawText(name, x, y - 30, 23, DARKBLUE); // Titel über der Matrix
    
    for (int r = 0; r < m->rows; r++) {
        for (int c = 0; c < m->cols; c++) {
            // Berechne die Position jeder einzelnen Zelle
            Rectangle cellRec = {(float)(x + c * (CELL + GAP)), (float)(y + r * (CELL + GAP)), CELL, CELL};
            
            bool isSelected = (editable && activeEditMatrix == m && editRow == r && editCol == c);

            // Hintergrund- und Rahmenfarbe je nach Auswahl anpassen
            DrawRectangleRounded(cellRec, 0.12f, 4, isSelected ? SKYBLUE : RAYWHITE);
            DrawRectangleRoundedLines(cellRec, 0.12f, 4, isSelected ? BLUE : LIGHTGRAY);
            
            char s[24];
            if (isSelected) {
                strncpy(s, editBuffer, sizeof(s)); // Zeige den Text, den der User gerade tippt
            } else {
                snprintf(s, sizeof(s), "%g", m->v[r][c]); // Zeige den gespeicherten Wert
            }
            s[23] = '\0';
            
            DrawText(s, (int)(cellRec.x + 10), (int)cellRec.y + 20, 17, DARKGRAY);
        }
    }
}