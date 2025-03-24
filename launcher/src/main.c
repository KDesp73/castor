#include <assert.h>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

static char* execute(const char* command)
{
    char buffer[128];
    char *result = NULL;
    size_t result_size = 0;
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t buffer_len = strlen(buffer);
        result = (char*) realloc(result, result_size + buffer_len + 1);
        if (!result) {
            pclose(pipe);
            return NULL;
        }
        strcpy(result + result_size, buffer);
        result_size += buffer_len;
    }

    pclose(pipe);
    return result;
}

int main(void)
{
    // Initialization
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Launcher");

    // Default settings
    bool fullscreen = false;

    Rectangle startButton = { SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT - 80, 140, 40 };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Handle input
        if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            CloseWindow();
            printf("Launching game with settings:\n");
            printf("Fullscreen: %s\n", fullscreen ? "ON" : "OFF");
            if(fullscreen) {
                execute("./game -F");
            } else {
                execute("./game");
            }
            return 0;
        }

        // Toggle fullscreen
        if (IsKeyPressed(KEY_F)) fullscreen = !fullscreen;

        // Draw UI
        BeginDrawing();
        ClearBackground(GetColor(0x282828FF));

        DrawText("Game Launcher", SCREEN_WIDTH / 2 - MeasureText("Game Launcher", 20) / 2, 20, 20, WHITE);

        // Fullscreen Checkbox
        DrawText("Fullscreen", 80, 80, 18, RAYWHITE);
        Rectangle checkbox = { 200, 80, 20, 20 };
        DrawRectangleLinesEx(checkbox, 2, WHITE);
        if (fullscreen) DrawRectangle(checkbox.x + 4, checkbox.y + 4, 12, 12, GREEN);
        if (CheckCollisionPointRec(GetMousePosition(), checkbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            fullscreen = !fullscreen;
        }


        // Start Game Button
        DrawRectangleRec(startButton, LIGHTGRAY);
        DrawText("Start Game", startButton.x + 10, startButton.y + 10, 20, DARKGRAY);

        EndDrawing();
    }

    assert(0 || "Should not reach here");
    CloseWindow();
    return 0;
}
