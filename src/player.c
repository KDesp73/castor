#include "player.h"
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
    free(*player);
    *player = NULL;
}

void PlayerMoveFront(Player* player)
{
    player->X += cos(player->angle * M_PI / 180.0) * player->speed;
    player->Y += sin(player->angle * M_PI / 180.0) * player->speed;
    
}
void PlayerMoveBack(Player* player)
{
    player->X -= cos(player->angle * M_PI / 180.0) * player->speed;
    player->Y -= sin(player->angle * M_PI / 180.0) * player->speed;
    
}
void PlayerRotateLeft(Player* player)
{
    player->angle -= 1;
    if (player->angle < 0) player->angle += 360;

}
void PlayerRotateRight(Player* player)
{
    player->angle += 1;
    if (player->angle >= 360) player->angle -= 360;
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
