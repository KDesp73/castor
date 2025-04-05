#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "context.h"


void castor_MoveFront(castor_Context* context, float deltaTime);
void castor_MoveBack(castor_Context* context, float deltaTime);
void castor_MoveLeft(castor_Context* context, float deltaTime);
void castor_MoveRight(castor_Context* context, float deltaTime);
void castor_RotateX(castor_Context* context, double delta, float deltaTime);
void castor_RotateY(castor_Context* context, double delta, float deltaTime);

#define COLLISION_TOLERANCE 0.1f
bool castor_CheckCollision(float newX, float newY, const castor_Context* ctx);


#endif // MOVEMENT_H
