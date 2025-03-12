#ifndef LEVEL_H
#define LEVEL_H

#include "context.h"

typedef void (*LevelLoader)(Context*);

static inline void LoadLevel(Context* ctx, LevelLoader loader)
{
    loader(ctx);
}

#endif // LEVEL_H
