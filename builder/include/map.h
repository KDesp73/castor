#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
static inline void MapPrint(int* map[], size_t rows, size_t cols)
{
    printf("{\n");
    for(size_t i = 0; i < rows; i++) {
        printf("\t{ ");
        for(size_t j = 0; j < cols; j++) {
            if(j != cols - 1)
                printf("%d, ", map[i][j]);
            else 
                printf("%d", map[i][j]);
        }
        if(i != rows - 1)
            printf(" },\n");
        else
            printf(" }\n");
    }
    printf("}\n");
}

static inline void MapDraw(int** map, size_t rows, size_t cols, size_t scale, Texture textures[])
{
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            int tile = map[i][j];
            int posX = j * scale;
            int posY = i * scale;
            
            Texture texture = textures[tile-1];
            if (tile != 0) {
                if (texture.id == 0) {
                    // If texture is missing, draw a placeholder text
                    const char* text = TextFormat("%d", tile);
                    int font_size = scale / 2;
                    int text_width = MeasureText(text, font_size);
                    int textX = posX + (scale - text_width) / 2;
                    int textY = posY + (scale - font_size) / 2;
                    DrawText(text, textX, textY, font_size, RAYWHITE);
                } else {
                    DrawTexturePro(
                            texture, 
                            (Rectangle){0, 0, texture.width, texture.height},  // Source
                            (Rectangle){posX, posY, scale, scale},  // Destination
                            (Vector2){0, 0},  // No offset
                            0.0f,  // No rotation
                            WHITE
                            );
                }
            }
        }
    }
}

static inline int **MapCreate(size_t rows, size_t cols) {
    int **map = (int **)malloc(rows * sizeof(int *));
    for (size_t i = 0; i < rows; i++) {
        map[i] = (int *)malloc(cols * sizeof(int));
        for (size_t j = 0; j < cols; j++) {
            map[i][j] = 0;
        }
    }
    return map;
}

static inline void MapFree(int **map, size_t rows) {
    for (size_t i = 0; i < rows; i++) free(map[i]);
    free(map);
}

static inline bool MapSave(int** map, size_t rows, size_t cols, const char* path)
{
    FILE* file = fopen(path, "w");
    if(!file) {
        fprintf(stderr, "Could not open file %s\n", path);
        return false;
    }

    fprintf(file, "%zu %zu\n", rows, cols);

    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < cols; j++) {
            fprintf(file, "%d ", map[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

static inline int** MapLoad(size_t* rows, size_t* cols, const char* path) 
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", path);
        return NULL;
    }

    // Read the first line to get rows and cols
    if (fscanf(file, "%zu %zu", rows, cols) != 2) {
        printf("Error: Invalid map format (missing dimensions)\n");
        fclose(file);
        return NULL;
    }

    // Allocate memory for the map
    int** map = (int**)malloc(*rows * sizeof(int*));
    if (!map) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    for (size_t i = 0; i < *rows; i++) {
        map[i] = (int*)malloc(*cols * sizeof(int));
        if (!map[i]) {
            printf("Error: Memory allocation failed\n");

            // Free already allocated rows before returning
            for (size_t j = 0; j < i; j++) {
                free(map[j]);
            }
            free(map);
            fclose(file);
            return NULL;
        }
    }

    // Read the map values into the allocated array
    for (size_t i = 0; i < *rows; i++) {
        for (size_t j = 0; j < *cols; j++) {
            if (fscanf(file, "%d", &map[i][j]) != 1) {
                printf("Error: Invalid map format at row %zu, col %zu\n", i, j);
                
                // Free memory before exiting
                for (size_t k = 0; k < *rows; k++) {
                    free(map[k]);
                }
                free(map);
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return map;
}


#endif // MAP_H


