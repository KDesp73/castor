#ifndef OBJECTS_H
#define OBJECTS_H

enum {
    SPRITE_BARREL,
    SPRITE_COLUMN,
    SPRITE_LAMP,
    SPRITE_GOBLIN,
    SPRITE_SKELETON,
    SPRITE_EYE,
    SPRITE_MUSHROOM
};

#define ENTITY_GOBLIN(x, y)   \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_GOBLIN,    \
            3,                \
            false             \
        ),                    \
        6.0f,                 \
        100,                  \
        0.5,                  \
        0.5,                  \
        4,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )
#define ENTITY_SKELETON(x, y) \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_SKELETON,  \
            3,                \
            false             \
        ),                    \
        7.0f,                 \
        100,                  \
        0.7,                  \
        0.4,                  \
        4,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )
#define ENTITY_MUSHROOM(x, y) \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_MUSHROOM,  \
            3,                \
            false             \
        ),                    \
        3.0f,                 \
        100,                  \
        0.3,                  \
        0.4,                  \
        6,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )
#define ENTITY_EYE(x, y)      \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_EYE,       \
            3,                \
            false             \
            ),                \
        5.0f,                 \
        100,                  \
        0.4,                  \
        0.6,                  \
        5,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )

#define SPRITE_COLUMN(x, y)   \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_MIDDLE,        \
        SPRITE_COLUMN,        \
        2,                    \
        true                  \
    )
#define SPRITE_LAMP(x, y)     \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_UP,            \
        SPRITE_LAMP,          \
        2,                    \
        false                 \
    )
#define SPRITE_BARREL(x, y)   \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_DOWN,          \
        SPRITE_BARREL,        \
        2,                    \
        true                  \
    )


#endif // OBJECTS_H
