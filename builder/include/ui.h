#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stddef.h>
void Grid(size_t rows, size_t cols);
void ShowSelectedAtCursor(size_t selected, int font_size, Color color);
void ShowCoordsAtCursor(Vector2 coords, int font_size, Color color);
Vector2 MouseCellPosition(size_t rows, size_t cols, size_t scale);
long Map(long x, long in_min, long in_max, long out_min, long out_max);
size_t Selected();

#endif // UI_H
