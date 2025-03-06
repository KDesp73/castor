#include "cli.h"
#include "map.h"
#include "textures.h"
#include "ui.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_TITLE "lvl - A level builder"
#define FPS 60

void AddBorder(size_t selected, int **map, size_t rows, size_t cols)
{
    for(size_t col = 0; col < cols; col++) {
        map[0][col] = selected;
        map[rows-1][col] = selected;
    }

    for(size_t row = 1; row < rows-1; row++) {
        map[row][0] = selected;
        map[row][cols-1] = selected;
    }
}

int main(int argc, char** argv)
{
    Input in = ParseCliArgs(argc, argv);

    int** map = NULL;
    if(in.loaded == NULL) {
        map = MapCreate(in.rows, in.cols);
    } else {
        map = MapLoad(&in.rows, &in.cols, in.loaded);
    }

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(in.cols * in.scale, in.rows * in.scale, WINDOW_TITLE);
    SetTargetFPS(FPS);

    Texture textures[MAX_TEXTURES];
    LoadTextures(textures, "textures.list");

    size_t selected = 0;
    while(!WindowShouldClose()){
        size_t new = Selected();
        if(new != 0) selected = new;
        
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 pos = MouseCellPosition(in.rows, in.cols, in.scale);
            map[(int)pos.y][(int)pos.x] = selected;
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            Vector2 pos = MouseCellPosition(in.rows, in.cols, in.scale);
            map[(int)pos.y][(int)pos.x] = 0;
        }

        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_S)) {
            MapSave(map, in.rows, in.cols, (in.output) ? in.output : "temp.lvl");
        }

        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_B)) {
            AddBorder(selected, map, in.rows, in.cols);
        }

        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));

        Grid(in.rows, in.cols);

        MapDraw(map, in.rows, in.cols, in.scale, textures);

        if(selected != 0) {
            ShowSelectedAtCursor(selected, 20, YELLOW);
        }


        EndDrawing();
    }
    MapPrint(map, in.rows, in.cols);
    MapFree(map, in.rows);

    if(in.output) free(in.output);
    if(in.loaded) free(in.loaded);

    return 0;
}
