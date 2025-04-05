#include "image.h"
#include <SDL2/SDL_image.h>

castor_Image castor_LoadImage(SDL_Renderer* renderer, const char* path)
{
    castor_Image image = {0};

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

void castor_RenderImage(SDL_Renderer* renderer, castor_Image* image, float scale, int x, int y)
{
    if (!image || !image->texture) return;

    SDL_Rect srcRect = { 0, 0, image->w, image->h };
    SDL_Rect dstRect = { x, y, image->w * scale, image->h * scale };

    SDL_RenderCopy(renderer, image->texture, &srcRect, &dstRect);
}

void castor_RenderRotatedImage(SDL_Renderer* renderer, castor_Image* image, float scale, int x, int y, double angle)
{
    SDL_Rect destRect = { x, y, image->w * scale, image->h * scale };

    SDL_Point center = { image->w / 2, image->h / 2 };

    SDL_RenderCopyEx(
        renderer,        // Renderer
        image->texture,  // Texture
        NULL,            // Source rect (NULL = full texture)
        &destRect,       // Destination rect
        angle,           // Rotation angle in degrees (clockwise)
        &center,         // Center of rotation (can be NULL = top-left)
        SDL_FLIP_NONE    // No flipping
    );
}

void castor_FreeImage(castor_Image* image)
{
    if (image->texture) {
        SDL_DestroyTexture(image->texture);
        image->texture = NULL;
    }
}
