#include "ui_components.h"


bool drawButton(Rectangle rect, const char *text, Color baseColor, bool isActive, Vector2 mousePos) {
    // Prüfen, ob die Maus über dem Button schwebt
    bool isHovered = CheckCollisionPointRec(mousePos, rect);
    
    // Farbe leicht anpassen bei Hover
    Color finalColor = isHovered ? Fade(baseColor, 0.8f) : baseColor;
    
    // Button-Hintergrund zeichnen
    DrawRectangleRounded(rect, 0.2f, 4, finalColor);
    
    // Wenn aktiv (z.B. ausgewählte Operation), zeichnen wir einen Rahmen
    if (isActive) {
        DrawRectangleRoundedLines(rect, 0.2f, 4, DARKBLUE);
    }
    
    // Text zentriert im Button zeichnen
    int textWidth = MeasureText(text, 18);
    int textX = (int)(rect.x + (rect.width - textWidth) / 2);
    int textY = (int)(rect.y + (rect.height - 18) / 2);
    
    DrawText(text, textX, textY, 18, RAYWHITE);
    
    // Gibt true zurück, wenn die Maus über dem Button ist UND geklickt wurde
    return (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}