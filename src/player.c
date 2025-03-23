#include "player.h"
#include <stdbool.h>
#include <stdlib.h>

void PlayerInit(Player* player, double speed, double angleDelta, double angle, double x, double y)
{
    player->speed = speed;
    player->angleDelta = angleDelta;
    player->angleX = angle;
    player->angleY = 0;
    player->X = x;
    player->Y = y;
}

Player* PlayerNew(double speed, double angleDelta, double angle, double x, double y)
{
    Player* result = (Player*) malloc(sizeof(Player));

    PlayerInit(result, speed, angleDelta, angle, x, y);

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
        .angleX = player->angleX,
        .speed = player->speed
    };
}
void PlayerLoad(Player* player, Player stored)
{
    player->X = stored.X;
    player->Y = stored.Y;
    player->angleX = stored.angleX;
    player->speed = stored.speed;
}

void PlayerPlace(Player* player, double x, double y, double angle)
{
    player->X = x;
    player->Y = y;
    player->angleX = angle;
}
