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
    if(IsKeyPressed(KEY_ZERO))  selected = 10;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_ONE))   selected = 11;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_TWO))   selected = 12;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_THREE)) selected = 13;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_FOUR))  selected = 14;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_FIVE))  selected = 15;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_SIX))   selected = 16;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_SEVEN)) selected = 17;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_EIGHT)) selected = 18;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_NINE))  selected = 19;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_ZERO))  selected = 20;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_ONE))   selected = 21;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_TWO))   selected = 22;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_THREE)) selected = 23;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_FOUR))  selected = 24;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_FIVE))  selected = 25;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_SIX))   selected = 26;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_SEVEN)) selected = 27;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_EIGHT)) selected = 28;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_NINE))  selected = 29;
    if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_ZERO))  selected = 30;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_ONE))   selected = 31;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_TWO))   selected = 32;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_THREE)) selected = 33;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_FOUR))  selected = 34;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_FIVE))  selected = 35;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_SIX))   selected = 36;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_SEVEN)) selected = 37;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_EIGHT)) selected = 38;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_NINE))  selected = 39;
    if((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_ZERO))  selected = 40;

    return selected;
}
