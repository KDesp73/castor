#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void MapPrint(int* map[], size_t rows, size_t cols);
int **MapCreate(size_t rows, size_t cols);
void MapFree(int **map, size_t rows);
bool MapSave(int** map, size_t rows, size_t cols, const char* path);
int** MapLoad(size_t* rows, size_t* cols, const char* path);
void MapCpy(int** src, int** dst, size_t w, size_t h);

#endif // MAP_H


