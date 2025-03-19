#include "context.h"
#include "entity.h"
#include "event.h"
#include "map.h"
#include "sprite.h"
#include "textures.h"
#include "ui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool ConstructRenderer(Context* ctx)
{
    ctx->window = SDL_CreateWindow(
            ctx->game_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ctx->screen_width, ctx->screen_height, SDL_WINDOW_SHOWN);

    if(!ctx->window) {
        fprintf(stderr, "Could not create window\n");
        return false;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);

    if(!ctx->renderer) {
        fprintf(stderr, "Could not create renderer\n");
        return false;
    }

    return true;
}

void ContextInit(Context* ctx)
{
    ctx->running = true;
    ctx->fov = 60;
    ctx->screen_width = DEFAULT_SCREEN_WIDTH;
    ctx->screen_height = DEFAULT_SCREEN_HEIGHT;
    ctx->texture_width = 64;
    ctx->texture_height = 64;
    ctx->mouse_sensitivity = 0.25;
    ctx->mouse_inverted = true;
    ctx->render_distance = 20.0f;

    ctx->enable_fog = true;
    ctx->fog_distance = 10;
}

void ContextFree(Context* ctx)
{
    if (ctx) {
        PlayerFree(&ctx->player);
        FreeTextures(ctx);
        FreeSprites(ctx);
        FreeEntities(ctx);
        FreeEvents(ctx);
        UIClose(&ctx->ui);
        MapFree(ctx->map, ctx->map_height);
        CleanupThreads(ctx);

        if (ctx->renderer) {
            SDL_DestroyRenderer(ctx->renderer);
            ctx->renderer = NULL;
        }

        if (ctx->window) {
            SDL_DestroyWindow(ctx->window);
            ctx->window = NULL;
        }
    }
}

void FreeTextures(Context* ctx)
{
    for(size_t i = 0; i < MAX_TEXTURES; i++) {
        if(ctx->textures[i] != NULL) {
            SDL_DestroyTexture(ctx->textures[i]); 
            ctx->textures[i] = NULL;
        }
        if(ctx->sprite_textures[i] != NULL) {
            SDL_DestroyTexture(ctx->sprite_textures[i]); 
            ctx->sprite_textures[i] = NULL;
        }
    }
    ctx->textures_loaded = false;
    ctx->sprites_loaded = false;
}
void FreeSprites(Context* ctx)
{
    for(size_t i = 0; i < ctx->sprite_count; i++){
        if(ctx->sprites[i]){
            SpriteFree(&ctx->sprites[i]);
        }
    }
}
void FreeEntities(Context* ctx)
{
    for(size_t i = 0; i < ctx->entity_count; i++){
        if(ctx->entities[i]){
            EntityFree(&ctx->entities[i]);
        }
    }
}
void FreeEvents(Context* ctx)
{
    for(size_t i = 0; i < ctx->event_count; i++){
        if(ctx->events[i]){
            EventFree(&ctx->events[i]);
        }
    }
}



void LoadLevelMap(Context* ctx, const char* path)
{
    ctx->map = MapLoad(&ctx->map_height, &ctx->map_width, path);
}

void LoadTextures(Context* ctx)
{
    TexturesLoad(ctx->renderer, ctx->textures, TEXTURES_LIST_FILE);
    ctx->textures_loaded = true;
    TexturesLoad(ctx->renderer, ctx->sprite_textures, SPRITES_LIST_FILE);
    ctx->sprites_loaded = true;
}

void AppendSprite(Context* ctx, Sprite* sprite)
{
    if(ctx->sprite_count >= MAX_SPRITES) return;

    ctx->sprites[ctx->sprite_count++] = sprite;
}

void AppendEntity(Context* ctx, Entity* entity)
{
    if (ctx->entity_count >= MAX_ENTITIES) return;

    AppendSprite(ctx, entity->sprite);
    entity->sprite = ctx->sprites[ctx->sprite_count - 1];

    ctx->entities[ctx->entity_count++] = entity;
}

void AppendEvent(Context* ctx, Event* evt)
{
    if(ctx->event_count >= MAX_EVENTS) return;

    ctx->events[ctx->event_count++] = evt;
}

int** ExportSearchMap(Context* ctx)
{
    int** res = MapCreate(ctx->map_height, ctx->map_width);
    if (!res) return NULL;

    // Fill based on original map
    for (size_t j = 0; j < ctx->map_height; j++) {
        for (size_t i = 0; i < ctx->map_width; i++) {
            res[j][i] = (ctx->map[j][i] != 0) ? 1 : 0;
        }
    }

    // Mark sprite collisions
    for (size_t k = 0; k < ctx->sprite_count; k++) {
        Sprite* sprite = ctx->sprites[k];

        if (sprite->collision) {
            int gridX = (int)floor(sprite->x);
            int gridY = (int)floor(sprite->y);

            if (gridX >= 0 && gridX < ctx->map_width && gridY >= 0 && gridY < ctx->map_height) {
                res[gridY][gridX] = 1;
            }
        }
    }

    return res;
}

void UpdateEntities(Context* ctx, float deltaTime)
{
    int** map = ExportSearchMap(ctx);
    
    for (size_t i = 0; i < ctx->entity_count; i++) {
        Entity* e = ctx->entities[i];
        if(!e) continue;
        if(e->health <= 0) {
            // TODO: Proper death method
            EntityFree(&e);
            continue;
        }

        if (e->move) {
            e->move(e, (const int**)map, ctx->map_width, ctx->map_height, ctx->player, deltaTime);
        }
    }

    MapFree(map, ctx->map_height);
}

void ProcessEvents(Context* ctx)
{
    Uint32 now = SDL_GetTicks();

    for(size_t i = 0; i < ctx->event_count; i++) {
        Event* ev = ctx->events[i];

        if (now - ev->last_processed >= ev->cooldown) {
            EventProcess(ev);
            ev->last_processed = now;
        }
    }
}
