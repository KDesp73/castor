#ifndef LEVEL_H
#define LEVEL_H

#include "context.h"

typedef void (*castor_LevelLoader)(castor_Context*);

static inline void castor_LoadLevel(castor_Context* ctx, castor_LevelLoader loader)
{
    loader(ctx);
}

#endif // LEVEL_H
