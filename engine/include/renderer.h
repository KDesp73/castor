#ifndef CASTOR_RENDERER_H
#define CASTOR_RENDERER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct castor_RenderContext castor_RenderContext;

typedef struct {
    bool (*init)(castor_RenderContext* ctx, int width, int height, const char* title, bool fullscreen);
    void (*shutdown)(castor_RenderContext* ctx);

    void (*clear)(castor_RenderContext* ctx);
    void (*present)(castor_RenderContext* ctx);

    void (*draw_texture)(castor_RenderContext* ctx, void* texture, int x, int y, int w, int h);
    void* (*load_texture)(castor_RenderContext* ctx, const char* path);
    void (*destroy_texture)(castor_RenderContext* ctx, void* texture);

    void (*set_fullscreen)(castor_RenderContext* ctx, bool enabled);
    void (*resize)(castor_RenderContext* ctx, int width, int height);
} castor_RenderAPI;

struct castor_RenderContext {
    void* user_data;      // Backend-specific data (e.g. SDL_Window/Renderer, OpenGL context)
    castor_RenderAPI api;
};

#endif
