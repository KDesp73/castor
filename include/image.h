#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL_render.h>
typedef struct {
    SDL_Texture* texture;
    int w, h;
} Image;
Image LoadImage(SDL_Renderer* renderer, const char* path);
void RenderImage(SDL_Renderer* renderer, Image* image, float scale, int x, int y);
void RenderRotatedImage(SDL_Renderer* renderer, Image* image, float scale, int x, int y, double angle);
void FreeImage(Image* image);


#endif //IMAGE_H
