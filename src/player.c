#include "player.h"
#include <stdbool.h>
#include <stdlib.h>

void castor_PlayerInit(castor_Player* player, double speed, double angleDelta)
{
    player->speed = speed;
    player->angleDelta = angleDelta;
    player->angleX = 0;
    player->angleY = 0;
    player->X = 0;
    player->Y = 0;
}

castor_Player* castor_PlayerNew(double speed, double angleDelta)
{
    castor_Player* result = (castor_Player*) malloc(sizeof(castor_Player));

    castor_PlayerInit(result, speed, angleDelta);

    return result;
}


void castor_PlayerFree(castor_Player** player)
{
    if(*player){
        free(*player);
        *player = NULL;
    }
}

castor_Player castor_PlayerStore(const castor_Player* player)
{
    return (castor_Player) {
        .X = player->X,
        .Y = player->Y,
        .angleX = player->angleX,
        .speed = player->speed
    };
}
void castor_PlayerLoad(castor_Player* player, castor_Player stored)
{
    player->X = stored.X;
    player->Y = stored.Y;
    player->angleX = stored.angleX;
    player->speed = stored.speed;
}

void castor_PlayerPlace(castor_Player* player, double x, double y, double angle)
{
    player->X = x;
    player->Y = y;
    player->angleX = angle;
}
