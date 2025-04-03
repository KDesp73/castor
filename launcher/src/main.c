#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

int main(void)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Launcher");

    bool fullscreen = false;
    bool debug = false;

    Rectangle startButton = { SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT - 80, 140, 40 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            printf("Launching game with settings:\n");
            printf("Fullscreen: %s\n", fullscreen ? "ON" : "OFF");
            printf("Debug: %s\n", debug ? "ON" : "OFF");

            char buffer[64];
            snprintf(buffer, sizeof(buffer), "./eidolon%s%s",
                     fullscreen ? " -F" : "",
                     debug ? " -D" : "");

            system(buffer);
            return 0;
        }

        if (IsKeyPressed(KEY_F)) fullscreen = !fullscreen;
        if (IsKeyPressed(KEY_D)) debug = !debug;

        BeginDrawing();
        ClearBackground(GetColor(0x282828FF));

        DrawText("Game Launcher", SCREEN_WIDTH / 2 - MeasureText("Game Launcher", 20) / 2, 20, 20, WHITE);

        // Fullscreen Checkbox
        DrawText("Fullscreen", 80, 80, 18, RAYWHITE);
        Rectangle checkbox = { 200, 80, 20, 20 };
        DrawRectangleLinesEx(checkbox, 2, WHITE);
        if (fullscreen) DrawRectangle(checkbox.x + 4, checkbox.y + 4, 12, 12, GREEN);
        if (CheckCollisionPointRec(GetMousePosition(), checkbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            fullscreen = !fullscreen;
        }

        // Debug Checkbox (Fixed)
        DrawText("Debug", 80, 160, 18, RAYWHITE);
        Rectangle checkbox1 = { 200, 160, 20, 20 };
        DrawRectangleLinesEx(checkbox1, 2, WHITE);
        if (debug) DrawRectangle(checkbox1.x + 4, checkbox1.y + 4, 12, 12, GREEN);  // FIXED: Uses `debug` now
        if (CheckCollisionPointRec(GetMousePosition(), checkbox1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            debug = !debug;
        }

        // Start Game Button
        DrawRectangleRec(startButton, LIGHTGRAY);
        DrawText("Start Game", startButton.x + 10, startButton.y + 10, 20, DARKGRAY);

        EndDrawing();
    }

    assert(0 && "Should not reach here");
    CloseWindow();
    return 0;
}
