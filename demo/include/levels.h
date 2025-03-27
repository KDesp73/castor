#ifndef LEVELS_H
#define LEVELS_H

#include "context.h"
#include "level.h"

void Level0(castor_Context* ctx);
void Level1(castor_Context* ctx);
void Level2(castor_Context* ctx);
void Level3(castor_Context* ctx);
void Level4(castor_Context* ctx);
void Level5(castor_Context* ctx); // Credits

static inline castor_LevelLoader Level(int index)
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
