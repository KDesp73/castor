#include "world.h"
#include <SDL2/SDL_rwops.h>
#include <assert.h>
#include <stdlib.h>

void castor_MapPrint(castor_Map map)
{
    printf("{\n");
    for(size_t i = 0; i < map.h; i++) {
        printf("\t{ ");
        for(size_t j = 0; j < map.w; j++) {
            if(j != map.w - 1)
                printf("%d, ", map.grid[i][j]);
            else 
                printf("%d", map.grid[i][j]);
        }
        if(i != map.h - 1)
            printf(" },\n");
        else
            printf(" }\n");
    }
    printf("}\n");
}

castor_Map* castor_MapCreate(size_t rows, size_t cols)
{
    castor_Map* map = malloc(sizeof(castor_Map));
    map->grid = (int **)malloc(rows * sizeof(int *));
    for (size_t i = 0; i < rows; i++) {
        map->grid[i] = (int *)malloc(cols * sizeof(int));
        for (size_t j = 0; j < cols; j++) {
            map->grid[i][j] = 0;
        }
    }
    map->w = cols;
    map->h = rows;
    return map;
}

void castor_MapFree(castor_Map** map)
{
    if(!*map) return;
    if(!(*map)->grid) return;
    for (size_t i = 0; i < (*map)->h; i++) 
        if((*map)->grid[i])
            free((*map)->grid[i]);
    free((*map)->grid);
    (*map)->grid = NULL;
    free(*map);
    *map = NULL;
}

bool castor_MapSave(const castor_Map* map, const char* path)
{
    assert(map);
    FILE* file = fopen(path, "w");
    if(!file) {
        fprintf(stderr, "Could not open file %s\n", path);
        return false;
    }

    fprintf(file, "%zu %zu\n", map->h, map->w);

    for(size_t i = 0; i < map->h; i++) {
        for(size_t j = 0; j < map->w; j++) {
            fprintf(file, "%d ", map->grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

castor_Map* castor_MapLoad(const char* path) 
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", path);
        return NULL;
    }

    castor_Map* map = malloc(sizeof(castor_Map));

    // Read the first line to get rows and cols
    if (fscanf(file, "%zu %zu", &map->h, &map->w) != 2) {
        printf("Error: Invalid map format (missing dimensions)\n");
        fclose(file);
        free(map);
        return NULL;
    }

    // Allocate memory for the map
    map->grid = (int**)malloc(map->h * sizeof(int*));
    if (!map->grid) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        free(map);
        return NULL;
    }

    for (size_t i = 0; i < map->h; i++) {
        map->grid[i] = (int*)malloc(map->w * sizeof(int));
        if (!map->grid[i]) {
            printf("Error: Memory allocation failed\n");

            // Free already allocated rows before returning
            for (size_t j = 0; j < i; j++) {
                free(map->grid[j]);
            }
            free(map->grid);
            free(map);
            fclose(file);
            return NULL;
        }
    }

    // Read the map values into the allocated array
    for (size_t i = 0; i < map->h; i++) {
        for (size_t j = 0; j < map->w; j++) {
            if (fscanf(file, "%d", &map->grid[i][j]) != 1) {
                printf("Error: Invalid map format at row %zu, col %zu\n", i, j);
                
                // Free memory before exiting
                for (size_t k = 0; k < map->h; k++) {
                    free(map->grid[k]);
                }
                free(map->grid);
                free(map);
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return map;
}

void castor_MapCpy(castor_Map* src, castor_Map* dst)
{
    if (src == NULL || dst == NULL) {
        return;
    }
    
    for (size_t y = 0; y < src->h; y++) {
        if (src->grid[y] == NULL || dst->grid[y] == NULL) {
            continue;
        }
        
        for (size_t x = 0; x < src->w; x++) {
            dst->grid[y][x] = src->grid[y][x];
        }
    }
}
