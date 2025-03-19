#include "events.h"
#include "context.h"
#include "player.h"
#include "sound.h"
#include "sprite.h"
#include <stdbool.h>
#include <stdio.h>

#define ATTACK_RANGE 3.5f
#define FACE_THRESHOLD 0.8f
#define FLOATP(x) printf(#x ": %f\n", x);

static bool PlayerIsAttacking(Player* player, Context* ctx)
{
    Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
    if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        return false;
    }

    // Normalize player direction vector
    float playerDirLength = sqrtf(player->angleX * player->angleX + player->angleY * player->angleY);
    if (playerDirLength == 0) return false; // Avoid division by zero
    float normAngleX = player->angleX / playerDirLength;
    float normAngleY = player->angleY / playerDirLength;
    FLOATP(normAngleY);

    // Iterate through entities to check if within range and facing the player
    for (size_t i = 0; i < ctx->entity_count; ++i) {
        Entity* entity = ctx->entities[i];

        if (entity == NULL || entity->sprite == NULL) {
            continue; // Skip invalid entities or entities without sprites
        }

        // Calculate distance to the entity
        float dx = entity->sprite->x - player->X;
        float dy = entity->sprite->y - player->Y;
        float distance = sqrtf(dx * dx + dy * dy);
        if (distance > ATTACK_RANGE) {
            continue; // Skip if the entity is too far
        }

        // Normalize entity direction
        dx /= distance;
        dy /= distance;

        // Dot product to check if facing the entity
        float dot = dx * normAngleX + dy * normAngleY;
        FLOATP(dot);
        if (dot > FACE_THRESHOLD) {
            return true; // Player is attacking this entity
        }
    }

    return false; // No entities were attacked
}

bool PlayerAttackTrigger(Event* evt)
{
    Context* ctx = evt->ctx;
    Player* player = ctx->player;
    
    return PlayerIsAttacking(player, ctx); // Returns true if the player is attacking
}

void PlayerAttackAction(Event* evt)
{
    Context* ctx = evt->ctx;
    Player* player = ctx->player;

    // Iterate through entities to apply damage
    for (size_t i = 0; i < ctx->entity_count; ++i) {
        Entity* entity = ctx->entities[i];

        if (entity == NULL || entity->sprite == NULL) {
            continue; // Skip invalid entities or entities without sprites
        }

        // Check if the player is in attack range of the entity
        if (player->X >= entity->sprite->x - 1 && player->X <= entity->sprite->x + 1 &&
            player->Y >= entity->sprite->y - 1 && player->Y <= entity->sprite->y + 1) {

            // Apply damage and play sound
            EntityTakeDamage(entity, 30);
            PlaySound(ctx, "./assets/sounds/sword-hit.mp3", 20, 200);

            // If the entity is dead, remove it
            if (entity->health <= 0) {
                RemoveEntity(ctx, entity);
            }

            break; // Only attack one entity at a time
        }
    }
}

bool trigger1(Event* evt)
{
    Context* ctx = evt->ctx;
    Player* player = ctx->player;

    return (
        (player->X >= 4 && player->X < 5) &&
        (player->Y >= 8 && player->Y < 9)
    );
}

void action1(Event* evt)
{
    Context* ctx = evt->ctx;

    UIToast toast = {0};
    UIToastInit(&toast, "Entered (4, 8) square", 2000);
    UIAppendToast(&ctx->ui, toast);
}




#define TOUCH_DISTANCE 0.5f
static bool PlayerTouchingSprite(const Player* player, const Sprite* sprite)
{
    if (!player || !sprite) return false;

    float dx = player->X - sprite->x;
    float dy = player->Y - sprite->y;
    float distance = sqrtf(dx * dx + dy * dy);

    return distance < TOUCH_DISTANCE;
}
static bool CheckItemPickup(Context* ctx)
{
    bool picked_up = false;
    for (size_t i = 0; i < ctx->item_count; i++) {
        Item* item = ctx->items[i];
        Sprite* sprite = item->sprite;

        if (PlayerTouchingSprite(ctx->player, sprite)) {
            RemoveItem(ctx, item);
            i--;
            picked_up = true;
        }
    }
    return picked_up;
}

bool PickItemTrigger(Event* evt)
{
    Context* ctx = evt->ctx;

    return CheckItemPickup(ctx);
}

void PickItemAction(Event* evt)
{
    // Since the picking-up is happening in the trigger we can do nothing here
    printf("Picked up an item!\n");
    // TODO: Add the item to an inventory
}
