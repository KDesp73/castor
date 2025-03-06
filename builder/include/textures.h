#ifndef TEXTURES_H
#define TEXTURES_H

#include "raylib.h"
#include <stdbool.h>
#define MAX_TEXTURES 256
#define MAX_PATH 512

bool LoadTextures(Texture2D textures[], const char* path);

#endif // TEXTURES_H
