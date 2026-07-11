#include "raylib.h"

int main(void){

    InitWindow(800,600,"Linear Algebra ToolBox");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Hallo Matrix Editor!", 250, 280, 30, DARKBLUE);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
