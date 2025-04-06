#include "events.h"
#include "core.h"
#include "game_player.h"
#include "ingame_ui.h"
#include "inventory.h"
#include "player.h"
#include <stdbool.h>
#include <stdio.h>
#include "world.h"

#define ATTACK_RANGE 3.0f
#define FACE_THRESHOLD 0.8f
#define FLOATP(x) printf(#x ": %f\n", x);

static bool PlayerIsAttacking(castor_Player* player, castor_Context* ctx)
{
    // Check if attack button is pressed (space or left mouse button)
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
    bool attacking = keys[SDL_SCANCODE_SPACE] || (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (!attacking) {
        return false;
    }

    // Calculate player direction vector based on angle
    float dirX = cosf(player->angleX * M_PI / 180.0f);
    float dirY = sinf(player->angleX * M_PI / 180.0f);

    // Iterate through entities
    for (size_t i = 0; i < ctx->level.entity_count; ++i) {
        castor_Entity* entity = ctx->level.entities[i];

        if (!entity || !entity->sprite) continue;

        float dx = entity->sprite->x - player->X;
        float dy = entity->sprite->y - player->Y;
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance > ATTACK_RANGE) continue; // Entity too far
        
        // Normalize direction to entity
        float normDx = dx / distance;
        float normDy = dy / distance;

        // Dot product to see if entity is in front
        float dot = dirX * normDx + dirY * normDy;

        // Face threshold (cosine of the angle difference, e.g., 0.7 means ~45 degrees cone)
        if (dot > FACE_THRESHOLD) {
            return true; // Attacking an entity within range & facing
        }
    }

    return false;
}

bool PlayerAttackTrigger(castor_Event* evt)
{
    if(!INV.sword) return false;

    castor_Context* ctx = evt->ctx;
    castor_Player* player = ctx->level.player;
    
    return PlayerIsAttacking(player, ctx);
}

static int calculateDamage(int base_damage, int variance) {
    int random_offset = (rand() % (2 * variance + 1)) - variance; // Random value from -variance to +variance
    return base_damage + random_offset;
}

void PlayerAttackAction(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;
    castor_Player* player = ctx->level.player;

    for (size_t i = 0; i < ctx->level.entity_count; ++i) {
        castor_Entity* entity = ctx->level.entities[i];

        if (entity == NULL || entity->sprite == NULL) {
            continue;
        }
        float dx = entity->sprite->x - player->X;
        float dy = entity->sprite->y - player->Y;
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance > ATTACK_RANGE) continue;


        // Apply damage and play sound
        float damage = calculateDamage(castor_EntityTakeDamage(entity, 30), 5);
        AddDamageNumber(ctx, ctx->sdl.screen_width / 2, ctx->sdl.screen_height / 2, damage);
        castor_PlaySound(ctx, "./assets/sounds/sword-hit.mp3", ctx->sound.volume, 200);

        // If the entity is dead, remove it
        if (entity->health <= 0) {
            castor_RemoveEntity(ctx, entity);
        }

        break; // Only attack one entity at a time
    }
}

static void AddToInventory(const castor_Item* item)
{
    if(!strcmp("key", item->id)) {
        INV.key = true;
    } else if(!strcmp("sword", item->id)) {
        INV.sword = true;
    } else if(!strcmp("glasses", item->id)) {
        INV.glasses = true;
    }
}

#define TOUCH_DISTANCE 1.0f
static bool PlayerTouchingSprite(const castor_Player* player, const castor_Sprite* sprite)
{
    if (!player || !sprite) return false;

    float dx = player->X - sprite->x;
    float dy = player->Y - sprite->y;
    float distance = sqrtf(dx * dx + dy * dy);

    return distance < TOUCH_DISTANCE;
}
static bool CheckItemPickup(castor_Context* ctx)
{
    bool picked_up = false;
    for (int i = ctx->level.item_count - 1; i >= 0; i--) {
        castor_Item* item = ctx->level.items[i];
        castor_Sprite* sprite = item->sprite;

        if (PlayerTouchingSprite(ctx->level.player, sprite)) {
            AddToInventory(item);
            castor_RemoveItem(ctx, item);
            picked_up = true;
            break;
        }
    }
    return picked_up;
}

bool PickItemTrigger(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;

    return CheckItemPickup(ctx);
}

void PickItemAction(castor_Event* evt)
{
    // Since the picking-up is happening 
    // in the trigger we can do nothing here
}


void DoorTooltipAction(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;

    UIToast toast = {0};
    UIToastInit(&toast, "Press [E] to unlock", 2000, ctx->sdl.screen_width - 270, ctx->sdl.screen_height - 160);
    UIAppendToast(&ctx->ui, toast);
}

void DoorKeyAction(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_E]) {
        ctx->level.next = true;
    }
}

bool EnemyAttackTrigger(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;
    castor_Player* player = ctx->level.player;

    // Iterate through entities to apply damage
    for (size_t i = 0; i < ctx->level.entity_count; ++i) {
        castor_Entity* entity = ctx->level.entities[i];

        if (entity == NULL || entity->sprite == NULL) {
            continue; // Skip invalid entities or entities without sprites
        }

        // Check if the player is in attack range of the entity
        if (player->X >= entity->sprite->x - MIN_STOP_DISTANCE && player->X <= entity->sprite->x + MIN_STOP_DISTANCE &&
            player->Y >= entity->sprite->y - MIN_STOP_DISTANCE && player->Y <= entity->sprite->y + MIN_STOP_DISTANCE) {

            float damage = castor_EntityDealDamage(entity);
            PLR.health -= damage;

            return true;
        }
    }

    return false;
}

void EnemyAttackAction(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;
    castor_PlaySound(ctx, "./assets/sounds/grunt.mp3", ctx->sound.volume, 1000);
    if(PLR.health <= 0) {
        ctx->level.fail = true;
    }
}

bool GlitchTrigger(castor_Event* evt)
{
    if(!INV.glasses) return false;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (!keys[SDL_SCANCODE_X]) return false;

    return true;
}

bool GlassesTipTrigger(castor_Event* evt)
{
    castor_Context* ctx = (evt)->ctx;                             
    castor_Player* player = ctx->level.player;                    
    return (                                               
        ((player->X) >= (1) && (player->X) < (1) + 1) && 
        ((player->Y) >= (11) && (player->Y) < (11) + 1)    
    );                                                     
}

void GlassesTipAction(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;

    UIToast toast = {0};
    UIToastInit(&toast, "Press [X] to activate the glasses", 5000, ctx->sdl.screen_width - 270, ctx->sdl.screen_height - 160);
    UIAppendToast(&ctx->ui, toast);
}

bool TeleportTrigger(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;
    castor_Player* player = ctx->level.player;

#define STEPPED_ON_SQUARE(_x, _y) \
        ((player->X) >= (_x) && (player->X) < (_x) + 1) && \
        ((player->Y) >= (_y) && (player->Y) < (_y) + 1)    

    if(STEPPED_ON_SQUARE(14, 25)) return true;
    if(STEPPED_ON_SQUARE(14, 26)) return true;
    if(STEPPED_ON_SQUARE(14, 27)) return true;
    if(STEPPED_ON_SQUARE(14, 28)) return true;
    if(STEPPED_ON_SQUARE(14, 29)) return true;
    if(STEPPED_ON_SQUARE(14, 30)) return true;
    if(STEPPED_ON_SQUARE(14, 31)) return true;

    return false;
}
void TeleportAction(castor_Event* evt)
{
    castor_Context* ctx = evt->ctx;
    castor_Player* player = ctx->level.player;
    
    castor_PlayerPlace(player, 27, 27, 180);
}

bool CreditsTrigger(castor_Event* evt)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    return keys[SDL_SCANCODE_G];
}
void CreditsAction(castor_Event* evt)
{
    INV.glasses = true;
}

bool EETrigger(castor_Event *evt)
{
    castor_Context* ctx = evt->ctx;
    return ctx->level.entity_count == 0;
}
void EEAction(castor_Event *evt)
{
    castor_Context* ctx = evt->ctx;
    castor_PlayerPlace(ctx->level.player, 28, 28, 0);
}
