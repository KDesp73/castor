#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "context.h"

Uint8 HandleInput(Context* ctx);

void MoveFront(Context* context);
void MoveBack(Context* context);
void MoveLeft(Context* context);
void MoveRight(Context* context);
void RotateX(Context* context, double delta);
void RotateY(Context* context, double delta);

#define COLLISION_TOLERANCE 0.1f
bool CheckCollision(float newX, float newY, const Context* ctx);


#endif // MOVEMENT_H
