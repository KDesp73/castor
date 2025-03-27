#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void castor_MapPrint(int* map[], size_t rows, size_t cols);
int **castor_MapCreate(size_t rows, size_t cols);
void castor_MapFree(int **map, size_t rows);
bool castor_MapSave(int** map, size_t rows, size_t cols, const char* path);
int** castor_MapLoad(size_t* rows, size_t* cols, const char* path);
void castor_MapCpy(int** src, int** dst, size_t w, size_t h);

#endif // MAP_H


