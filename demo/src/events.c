#include "events.h"
#include "context.h"
#include "player.h"
#include "sound.h"
#include <stdbool.h>
#include <stdio.h>

#define ATTACK_RANGE 3.5f
#define FACE_THRESHOLD 0.8f
#define FLOATP(x) printf(#x ": %f\n", x);

bool PlayerIsAttacking(Player* player, Context* ctx)
{
    Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
    if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        return false;
    }

    float playerDirLength = sqrtf(player->angleX * player->angleX + player->angleY * player->angleY);
    float normAngleX = player->angleX / playerDirLength;
    float normAngleY = player->angleY / playerDirLength;
    FLOATP(normAngleY);

    for (int i = 0; i < ctx->entity_count; ++i) {
        Entity* entity = ctx->entities[i];

        float dx = entity->sprite->x - player->X;
        float dy = entity->sprite->y - player->Y;
        float distance = sqrtf(dx * dx + dy * dy);
        float adjustedRange = ATTACK_RANGE;

        FLOATP(distance);
        if (distance > adjustedRange) {
            continue;
        }

        dx /= distance;
        dy /= distance;

        float dot = dx * normAngleX + dy * normAngleY;
        FLOATP(dot);
        if (dot > FACE_THRESHOLD) {
            return true;
        }

    }

    return false;
}

bool PlayerAttackTrigger(Event* evt)
{
    Context* ctx = evt->ctx;
    Player* player = ctx->player;
    
    return PlayerIsAttacking(player, ctx);
}

void PlayerAttackAction(Event* evt)
{
    Context* ctx = evt->ctx;
    Player* player = ctx->player;

    for (int i = 0; i < ctx->entity_count; ++i) {
        Entity* entity = ctx->entities[i];

        if (player->X >= entity->sprite->x - 1 && player->X <= entity->sprite->x + 1 &&
            player->Y >= entity->sprite->y - 1 && player->Y <= entity->sprite->y + 1) {

            EntityTakeDamage(entity, 30);

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


