#include "player.h"
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
    free(*player);
    *player = NULL;
}
