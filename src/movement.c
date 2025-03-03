#include "movement.h"
#include "context.h"

void MoveFront(Context* ctx)
{
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * ctx->player->speed;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * ctx->player->speed;

    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveBack(Context* ctx)
{
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * -ctx->player->speed;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * -ctx->player->speed;

    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

#define ANGLE_DELTA 0.5
void RotateLeft(Context* ctx)
{
    ctx->player->angleX -= ANGLE_DELTA;
    if (ctx->player->angleX < 0) ctx->player->angleX += 360;

}
void RotateRight(Context* ctx)
{
    ctx->player->angleX += ANGLE_DELTA;
    if (ctx->player->angleX >= 360) ctx->player->angleX -= 360;
}

#define ANGLE_Y_CUTOFF 120
void LookUp(Context *ctx)
{
    if (ctx->player->angleY - ANGLE_DELTA < -ANGLE_Y_CUTOFF)
        ctx->player->angleY = -ANGLE_Y_CUTOFF;
    else
        ctx->player->angleY -= ANGLE_DELTA;
}

void LookDown(Context *ctx)
{
    if (ctx->player->angleY + ANGLE_DELTA > ANGLE_Y_CUTOFF)
        ctx->player->angleY = ANGLE_Y_CUTOFF;
    else
        ctx->player->angleY += ANGLE_DELTA;
}
