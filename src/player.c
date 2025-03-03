#include "player.h"
#include "context.h"
#include <math.h>
#include <stdlib.h>

void PlayerInit(Player* player, double speed, double angle, double x, double y)
{
    player->speed = speed;
    player->angle = angle;
    player->X = x;
    player->Y = y;
}

Player* PlayerNew(double speed, double angle, double x, double y)
{
    Player* result = (Player*) malloc(sizeof(Player));

    PlayerInit(result, speed, angle, x, y);

    return result;
}


void PlayerFree(Player** player)
{
    if(*player){
        free(*player);
        *player = NULL;
    }
}

Player PlayerStore(const Player* player)
{
    return (Player) {
        .X = player->X,
        .Y = player->Y,
        .angle = player->angle,
        .speed = player->speed
    };
}
void PlayerLoad(Player* player, Player stored)
{
    player->X = stored.X;
    player->Y = stored.Y;
    player->angle = stored.angle;
    player->speed = stored.speed;
}
