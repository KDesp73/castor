#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL_render.h>
typedef struct {
    SDL_Texture* texture;
    int w, h;
} castor_Image;

castor_Image castor_LoadImage(SDL_Renderer* renderer, const char* path);
void castor_RenderImage(SDL_Renderer* renderer, castor_Image* image, float scale, int x, int y);
void castor_RenderRotatedImage(SDL_Renderer* renderer, castor_Image* image, float scale, int x, int y, double angle);
void castor_FreeImage(castor_Image* image);


#endif //IMAGE_H
