#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    double speed;
    double angle;
    double X;
    double Y;
} Player;

void PlayerInit(Player* player, double speed, double angle, double x, double y);
Player* PlayerNew(double speed, double angle, double x, double y);
void PlayerFree(Player** player);


#endif // PLAYER_H
