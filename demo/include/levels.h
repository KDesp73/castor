#ifndef LEVELS_H
#define LEVELS_H

#include "context.h"
#include "level.h"

void Level0(Context* ctx);
void Level1(Context* ctx);
void Level2(Context* ctx);
void Level3(Context* ctx);
void Level4(Context* ctx);
void Level5(Context* ctx); // Credits

static inline LevelLoader Level(int index)
{
#define LVL(x) case x: return Level##x
    switch (index) {
        LVL(0);
        LVL(1);
        LVL(2);
        LVL(3);
        LVL(4);
        LVL(5);
    }
    return NULL;
}

#endif // LEVELS_H
