#include "map.h"

void MapPrint(int* map[], size_t rows, size_t cols)
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

int **MapCreate(size_t rows, size_t cols)
{
    int **map = (int **)malloc(rows * sizeof(int *));
    for (size_t i = 0; i < rows; i++) {
        map[i] = (int *)malloc(cols * sizeof(int));
        for (size_t j = 0; j < cols; j++) {
            map[i][j] = 0;
        }
    }
    return map;
}

void MapFree(int **map, size_t rows)
{
    if(!map) return;
    for (size_t i = 0; i < rows; i++) 
        free(map[i]);
    free(map);
}

bool MapSave(int** map, size_t rows, size_t cols, const char* path)
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

int** MapLoad(size_t* rows, size_t* cols, const char* path) 
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

void MapCpy(int** src, int** dst, size_t w, size_t h)
{
    if (src == NULL || dst == NULL) {
        return;
    }
    
    for (size_t y = 0; y < h; y++) {
        if (src[y] == NULL || dst[y] == NULL) {
            continue;
        }
        
        for (size_t x = 0; x < w; x++) {
            dst[y][x] = src[y][x];
        }
    }
}
