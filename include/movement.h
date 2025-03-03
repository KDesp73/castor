#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "context.h"
void MoveFront(Context* context);
void MoveBack(Context* context);
void RotateLeft(Context* context);
void RotateRight(Context* context);
void LookUp(Context* context);
void LookDown(Context* context);

bool CheckCollision(float newX, float newY, const Context* ctx);


#endif // MOVEMENT_H
