#include "raylib.h"
#include "drawMatrix.h"

#define TAB_COUNT 5



//Unsere Tabs für die verschiedenen Anwendungen
typedef enum {CALC, DET, INV, RANK, EIG} Tab;

int main(void){

    InitWindow(1120,1100,"Linear Algebra ToolBox");
    SetTargetFPS(60);

    MatrixData matrixA = {3, 3, {{0}}};

    //Wir starten beim Register "Rechnen"
    Tab aktuellerTab = CALC;
    const char *tabNamen[TAB_COUNT] = {"Rechnen", "Determinante", "Inverse", "Rang", "Eigenwerte"};

    while(!WindowShouldClose()){
        Vector2 mouse = GetMousePosition();
        
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            for(int i = 0; i< TAB_COUNT; i++){
                Rectangle r = {(float)(25+i*190), 65, 175, 38};
                if(CheckCollisionPointRec(mouse, r)){
                    aktuellerTab = (Tab)i;
                }
            }
        }
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Prüfen, ob eine Zelle von Matrix A angeklickt wurde (Position X: 50, Y: 100)
            handleMatrixClick(&matrixA, 50, 200, mouse);
          
        }

        handleKeyboardInput();
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        ClearBackground((Color){243, 247, 252, 255});
        drawMatrix(&matrixA, 50, 200, "Matrix A (Klicke Zelle zum Editieren)", true);
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
