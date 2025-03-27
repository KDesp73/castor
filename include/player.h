#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    double speed;
    double angleDelta;
    double angleX, angleY;
    double X, Y;
} castor_Player;

void castor_PlayerInit(castor_Player* player, double speed, double angleDelta, double angle, double x, double y);
castor_Player* castor_PlayerNew(double speed, double angleDelta, double angle, double x, double y);
void castor_PlayerFree(castor_Player** player);

castor_Player castor_PlayerStore(const castor_Player* player);
void castor_PlayerPlace(castor_Player* player, double x, double y, double angle);
void castor_PlayerLoad(castor_Player* player, castor_Player stored);

#endif // PLAYER_H
