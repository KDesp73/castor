#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    double speed;
    double angleDelta;
    double angleX, angleY;
    double X, Y;
} Player;

void PlayerInit(Player* player, double speed, double angleDelta, double angle, double x, double y);
Player* PlayerNew(double speed, double angleDelta, double angle, double x, double y);
void PlayerFree(Player** player);

Player PlayerStore(const Player* player);
void PlayerPlace(Player* player, double x, double y, double angle);
void PlayerLoad(Player* player, Player stored);

#endif // PLAYER_H
