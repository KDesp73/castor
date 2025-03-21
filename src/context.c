#include "context.h"
#include "entity.h"
#include "event.h"
#include "item.h"
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
#include <assert.h>

bool ConstructRenderer(Context* ctx)
{
    ctx->sdl.window = SDL_CreateWindow(
            ctx->game_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ctx->sdl.screen_width, ctx->sdl.screen_height, SDL_WINDOW_SHOWN);

    if(!ctx->sdl.window) {
        fprintf(stderr, "Could not create window\n");
        return false;
    }

    ctx->sdl.renderer = SDL_CreateRenderer(ctx->sdl.window, -1, SDL_RENDERER_ACCELERATED);

    if(!ctx->sdl.renderer) {
        fprintf(stderr, "Could not create renderer\n");
        return false;
    }

    return true;
}

void ContextInit(Context* ctx)
{
    ctx->engine.running = true;
    ctx->settings.fov = 60;
    ctx->sdl.screen_width = DEFAULT_SCREEN_WIDTH;
    ctx->sdl.screen_height = DEFAULT_SCREEN_HEIGHT;
    ctx->raycaster.texture_width = 64;
    ctx->raycaster.texture_height = 64;
    ctx->settings.mouse_sensitivity = 0.25;
    ctx->settings.mouse_inverted = true;
    ctx->settings.render_distance = 20.0f;

    ctx->settings.enable_fog = true;
    ctx->settings.fog_distance = 10;
}

void ContextFree(Context* ctx)
{
    if (ctx) {
        PlayerFree(&ctx->level.player);
        FreeTextures(ctx);
        FreeSprites(ctx);
        FreeEntities(ctx);
        FreeItems(ctx);
        FreeEvents(ctx);
        UIClose(&ctx->ui);
        MapFree(ctx->level.map, ctx->level.map_height);
        CleanupThreads(ctx);

        if (ctx->sdl.renderer) {
            SDL_DestroyRenderer(ctx->sdl.renderer);
            ctx->sdl.renderer = NULL;
        }

        if (ctx->sdl.window) {
            SDL_DestroyWindow(ctx->sdl.window);
            ctx->sdl.window = NULL;
        }
    }
}

void FreeTextures(Context* ctx)
{
    for(size_t i = 0; i < MAX_TEXTURES; i++) {
        if(ctx->raycaster.textures[i] != NULL) {
            SDL_DestroyTexture(ctx->raycaster.textures[i]); 
            ctx->raycaster.textures[i] = NULL;
        }
        if(ctx->raycaster.sprite_textures[i] != NULL) {
            SDL_DestroyTexture(ctx->raycaster.sprite_textures[i]); 
            ctx->raycaster.sprite_textures[i] = NULL;
        }
    }
    ctx->raycaster.textures_loaded = false;
    ctx->raycaster.sprites_loaded = false;
}
void FreeSprites(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.sprite_count; i++){
        if(ctx->level.sprites[i]){
            SpriteFree(&ctx->level.sprites[i]);
        }
    }
}
void FreeEntities(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.entity_count; i++){
        if(ctx->level.entities[i]){
            EntityFree(&ctx->level.entities[i]);
        }
    }
}
void FreeItems(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.item_count; i++){
        if(ctx->level.items[i]){
            ItemFree(&ctx->level.items[i]);
        }
    }
}

void FreeEvents(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.event_count; i++){
        if(ctx->level.events[i]){
            EventFree(&ctx->level.events[i]);
        }
    }
}



void LoadLevelMap(Context* ctx, const char* path)
{
    ctx->level.map = MapLoad(&ctx->level.map_height, &ctx->level.map_width, path);
}

void LoadTextures(Context* ctx)
{
    TexturesLoad(ctx->sdl.renderer, ctx->raycaster.textures, TEXTURES_LIST_FILE);
    ctx->raycaster.textures_loaded = true;
    TexturesLoad(ctx->sdl.renderer, ctx->raycaster.sprite_textures, SPRITES_LIST_FILE);
    ctx->raycaster.sprites_loaded = true;
}

void AppendSprite(Context* ctx, Sprite* sprite)
{
    if (ctx->level.sprite_count >= MAX_SPRITES) return;

    sprite->index = ctx->level.sprite_count;
    ctx->level.sprites[ctx->level.sprite_count++] = sprite;
    printf("Appended sprite at index: %zu\n", sprite->index);
}

void AppendEntity(Context* ctx, Entity* entity)
{
    if (ctx->level.entity_count >= MAX_ENTITIES) return;

    AppendSprite(ctx, entity->sprite);

    entity->index = ctx->level.entity_count;
    ctx->level.entities[ctx->level.entity_count++] = entity;
    printf("Appended entity '%s' at index: %zu\n", entity->id, entity->index);
}

void AppendItem(Context* ctx, Item* item)
{
    if (ctx->level.item_count >= MAX_ITEMS) return;

    AppendSprite(ctx, item->sprite);

    item->index = ctx->level.item_count;
    ctx->level.items[ctx->level.item_count++] = item;
    printf("Appended item '%s' at index: %zu\n", item->id, item->index);
}

void AppendEvent(Context* ctx, Event* evt)
{
    if(ctx->level.event_count >= MAX_EVENTS) return;

    ctx->level.events[ctx->level.event_count++] = evt;
}

int** ExportSearchMap(Context* ctx)
{
    int** res = MapCreate(ctx->level.map_height, ctx->level.map_width);
    if (!res) return NULL;

    // Fill based on original map
    for (size_t j = 0; j < ctx->level.map_height; j++) {
        for (size_t i = 0; i < ctx->level.map_width; i++) {
            res[j][i] = (ctx->level.map[j][i] != 0) ? 1 : 0;
        }
    }

    // Mark sprite collisions
    for (size_t k = 0; k < ctx->level.sprite_count; k++) {
        Sprite* sprite = ctx->level.sprites[k];

        if (sprite->collision) {
            int gridX = (int)floor(sprite->x);
            int gridY = (int)floor(sprite->y);

            if (gridX >= 0 && gridX < ctx->level.map_width && gridY >= 0 && gridY < ctx->level.map_height) {
                res[gridY][gridX] = 1;
            }
        }
    }

    return res;
}

void UpdateEntities(Context* ctx, float deltaTime)
{
    int** map = ExportSearchMap(ctx);
    
    for (size_t i = 0; i < ctx->level.entity_count; i++) {
        Entity* e = ctx->level.entities[i];
        if(!e) continue;
        if(e->health <= 0) {
            // TODO: Proper death method
            EntityFree(&e);
            continue;
        }

        if (e->move) {
            e->move(e, (const int**)map, ctx->level.map_width, ctx->level.map_height, ctx->level.player, deltaTime);
        }
    }

    MapFree(map, ctx->level.map_height);
}

void ProcessEvents(Context* ctx)
{
    Uint32 now = SDL_GetTicks();

    for(size_t i = 0; i < ctx->level.event_count; i++) {
        Event* ev = ctx->level.events[i];

        if (now - ev->last_processed >= ev->cooldown) {
            EventProcess(ev);
            ev->last_processed = now;
        }
    }
}

void RemoveSprite(Context* ctx, const Sprite* sprite)
{
    assert(ctx);
    assert(sprite);

    size_t index = sprite->index;
    printf("Removing sprite from index: %zu\n", index);
    if (index >= ctx->level.sprite_count) {
        fprintf(stderr, "Error: Invalid sprite index!\n");
        return;
    }

    SpriteFree(&ctx->level.sprites[index]);

    // Shift sprites left
    for (size_t i = index; i < ctx->level.sprite_count - 1; i++) {
        ctx->level.sprites[i] = ctx->level.sprites[i + 1];
        ctx->level.sprites[i]->index = i;
    }

    ctx->level.sprites[ctx->level.sprite_count - 1] = NULL;
    ctx->level.sprite_count--;
}

void RemoveEntity(Context* ctx, const Entity* entity)
{
    assert(ctx);
    assert(entity);

    size_t index = entity->index;
    printf("Removing entity '%s' from index: %zu\n", entity->id, index);
    if (index >= ctx->level.entity_count) {
        fprintf(stderr, "Error: Invalid entity index!\n");
        return;
    }

    // Remove associated sprite if available
    if (entity->sprite) {
        RemoveSprite(ctx, entity->sprite);
    }
    EntityFree(&ctx->level.entities[index]);

    // Shift entities left
    for (size_t i = index; i < ctx->level.entity_count - 1; i++) {
        ctx->level.entities[i] = ctx->level.entities[i + 1];
        ctx->level.entities[i]->index = i;
    }

    ctx->level.entities[ctx->level.entity_count - 1] = NULL;
    ctx->level.entity_count--;
}

void RemoveItem(Context* ctx, const Item* item)
{
    assert(ctx);
    assert(item);

    size_t index = item->index;
    printf("Removing item '%s' from index: %zu\n", item->id, index);
    if (index >= ctx->level.item_count) {
        fprintf(stderr, "Error: Invalid item index!\n");
        return;
    }

    if (item->sprite) {
        RemoveSprite(ctx, item->sprite);
    }
    ItemFree(&ctx->level.items[index]);

    for (size_t i = index; i < ctx->level.item_count - 1; i++) {
        if (ctx->level.items[i + 1]) {
            ctx->level.items[i] = ctx->level.items[i + 1];
            ctx->level.items[i]->index = i;
        } else {
            ctx->level.items[i] = NULL;
        }
    }

    ctx->level.items[ctx->level.item_count - 1] = NULL;
    ctx->level.item_count--;
}

static float PseudoRandomValue(uintptr_t seed)
{
    seed = (seed ^ 0xDEADBEEF) + (seed << 4);
    seed = seed ^ (seed >> 7);
    return (float)(seed % 1000) / 1000.0f;
}

void ItemsIdle(Context* ctx, float elapsedTime)
{
    float baseAmplitude = 1.0f;
    float baseSpeed = 2.0f;

    for (size_t i = 0; i < ctx->level.item_count; i++) {
        Item* item = ctx->level.items[i];
        if (item && item->sprite) {
            uintptr_t seed = (uintptr_t)item;
            float amplitude = baseAmplitude * (0.75f + 0.5f * PseudoRandomValue(seed));
            float speed = baseSpeed * (0.75f + 0.5f * PseudoRandomValue(seed + 1234));
            float phase = 2.0f * M_PI * PseudoRandomValue(seed + 5678);

            float offset = sinf(elapsedTime * speed + phase) * amplitude;
            item->sprite->z = item->baseZ + offset;
        }
    }
}
