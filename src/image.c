#include "image.h"
#include <SDL2/SDL_image.h>

Image LoadImage(SDL_Renderer* renderer, const char* path)
{
    Image image = {0};

    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return image;
    }

    image.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!image.texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return image;
    }

    SDL_QueryTexture(image.texture, NULL, NULL, &image.w, &image.h);

    return image;
}

void RenderImage(SDL_Renderer* renderer, Image* image, float scale, int x, int y)
{
    if (!image || !image->texture) return;

    SDL_Rect srcRect = { 0, 0, image->w, image->h };
    SDL_Rect dstRect = { x, y, image->w * scale, image->h * scale };

    SDL_RenderCopy(renderer, image->texture, &srcRect, &dstRect);
}

void FreeImage(Image* image)
{
    if (image->texture) {
        SDL_DestroyTexture(image->texture);
        image->texture = NULL;
    }
}
