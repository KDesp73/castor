#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "context.h"


void MoveFront(Context* context, float deltaTime);
void MoveBack(Context* context, float deltaTime);
void MoveLeft(Context* context, float deltaTime);
void MoveRight(Context* context, float deltaTime);
void RotateX(Context* context, double delta, float deltaTime);
void RotateY(Context* context, double delta, float deltaTime);

#define COLLISION_TOLERANCE 0.1f
bool CheckCollision(float newX, float newY, const Context* ctx);


#endif // MOVEMENT_H
