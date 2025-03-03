#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    double speed;
    double angle;
    double X, Y;
} Player;

void PlayerInit(Player* player, double speed, double angle, double x, double y);
Player* PlayerNew(double speed, double angle, double x, double y);
void PlayerFree(Player** player);

void PlayerMoveFront(Player* player);
void PlayerMoveBack(Player* player);
void PlayerRotateLeft(Player* player);
void PlayerRotateRight(Player* player);

Player PlayerStore(const Player* player);
void PlayerLoad(Player* player, Player stored);

#endif // PLAYER_H
