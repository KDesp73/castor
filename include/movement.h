#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "context.h"

Uint8 HandleInput(Context* ctx);

void MoveFront(Context* context);
void MoveBack(Context* context);
void MoveLeft(Context* context);
void MoveRight(Context* context);
void RotateLeft(Context* context, double delta);
void RotateRight(Context* context, double delta);
void LookUp(Context* context, double delta);
void LookDown(Context* context, double delta);

bool CheckCollision(float newX, float newY, const Context* ctx);


#endif // MOVEMENT_H
