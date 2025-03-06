#include "ui.h"
#include "raylib.h"
#include "raymath.h"
#include <stddef.h>

void Grid(size_t rows, size_t cols)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;

    // Draw vertical lines
    for (size_t col = 0; col <= cols; col++) {
        float x = col * cellWidth;
        DrawLine((int)x, 0, (int)x, screenHeight, LIGHTGRAY);
    }

    // Draw horizontal lines
    for (size_t row = 0; row <= rows; row++) {
        float y = row * cellHeight;
        DrawLine(0, (int)y, screenWidth, (int)y, LIGHTGRAY);
    }
}

long Map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Vector2 MouseCellPosition(size_t rows, size_t cols, size_t scale) 
{
    // Map mouse coordinates to cell grid
    int x_cell_over = (int)Map((float)GetMousePosition().x, 0, (float)(cols * scale), 0, (float) cols);
    x_cell_over = Clamp(x_cell_over, 0, cols - 1);

    int y_cell_over = (int)Map((float)GetMousePosition().y, 0, (float)(rows * scale), 0, (float) rows);
    y_cell_over = Clamp(y_cell_over, 0, rows - 1);

    Vector2 result;
    result.x = x_cell_over;
    result.y = y_cell_over;
    return result;
}

void ShowCoordsAtCursor(Vector2 coords, int font_size, Color color)
{
    const char* text = TextFormat("%.0f %.0f", coords.x, coords.y);
    DrawText(text, GetMousePosition().x + 20, GetMousePosition().y, font_size, color);
}
void ShowSelectedAtCursor(size_t selected, int font_size, Color color)
{
    const char* text = TextFormat("%zu", selected);
    DrawText(text, GetMousePosition().x + 20, GetMousePosition().y, font_size, color);
}

// NOTE: The tool supports 35 different textures / sprites 1-9 A-Z
size_t Selected()
{
    size_t selected = 0;

    if(IsKeyPressed(KEY_ONE))   selected = 1;
    if(IsKeyPressed(KEY_TWO))   selected = 2;
    if(IsKeyPressed(KEY_THREE)) selected = 3;
    if(IsKeyPressed(KEY_FOUR))  selected = 4;
    if(IsKeyPressed(KEY_FIVE))  selected = 5;
    if(IsKeyPressed(KEY_SIX))   selected = 6;
    if(IsKeyPressed(KEY_SEVEN)) selected = 7;
    if(IsKeyPressed(KEY_EIGHT)) selected = 8;
    if(IsKeyPressed(KEY_NINE))  selected = 9;
    if(IsKeyPressed(KEY_A))     selected = 10;
    if(IsKeyPressed(KEY_B))     selected = 11;
    if(IsKeyPressed(KEY_C))     selected = 12;
    if(IsKeyPressed(KEY_D))     selected = 13;
    if(IsKeyPressed(KEY_E))     selected = 14;
    if(IsKeyPressed(KEY_F))     selected = 15;
    if(IsKeyPressed(KEY_G))     selected = 16;
    if(IsKeyPressed(KEY_H))     selected = 17;
    if(IsKeyPressed(KEY_I))     selected = 18;
    if(IsKeyPressed(KEY_J))     selected = 19;
    if(IsKeyPressed(KEY_K))     selected = 20;
    if(IsKeyPressed(KEY_L))     selected = 21;
    if(IsKeyPressed(KEY_M))     selected = 22;
    if(IsKeyPressed(KEY_N))     selected = 23;
    if(IsKeyPressed(KEY_O))     selected = 24;
    if(IsKeyPressed(KEY_P))     selected = 25;
    if(IsKeyPressed(KEY_Q))     selected = 26;
    if(IsKeyPressed(KEY_R))     selected = 27;
    if(IsKeyPressed(KEY_S))     selected = 28;
    if(IsKeyPressed(KEY_T))     selected = 29;
    if(IsKeyPressed(KEY_U))     selected = 30;
    if(IsKeyPressed(KEY_V))     selected = 31;
    if(IsKeyPressed(KEY_W))     selected = 32;
    if(IsKeyPressed(KEY_X))     selected = 33;
    if(IsKeyPressed(KEY_Y))     selected = 34;
    if(IsKeyPressed(KEY_Z))     selected = 35;

    return selected;
}
