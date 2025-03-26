#ifndef OBJECTS_H
#define OBJECTS_H

enum {
    SPRITE_BARREL,
    SPRITE_COLUMN,
    SPRITE_LAMP,
    SPRITE_GOBLIN,
    SPRITE_SKELETON,
    SPRITE_EYE,
    SPRITE_MUSHROOM,
    SPRITE_KEY,
    SPRITE_GLASSES,
    SPRITE_SWORD,
    SPRITE_GRIMM,
};

#define ENTITY_GRIMM(x, y)    \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            0,                \
            SPRITE_GRIMM,     \
            2,                \
            false,            \
            "s_grimm" \
        ),                    \
        6.0f,                 \
        100,                  \
        0.9,                  \
        0.9,                  \
        6,                    \
        1.0f,                 \
        MoveSmoothAStar,      \
        "grimm"               \
    )

#define ENTITY_GOBLIN(x, y)   \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            0,                \
            SPRITE_GOBLIN,    \
            3,                \
            false,            \
            "s_goblin" \
        ),                    \
        6.0f,                 \
        100,                  \
        0.5,                  \
        0.5,                  \
        4,                    \
        1.0f,                 \
        MoveSmoothAStar,      \
        "goblin"              \
    )

#define ENTITY_SKELETON(x, y) \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            0,                \
            SPRITE_SKELETON,  \
            3,                \
            false,            \
            "s_skeleton" \
        ),                    \
        7.0f,                 \
        100,                  \
        0.7,                  \
        0.4,                  \
        4,                    \
        1.0f,                 \
        MoveSmoothAStar,      \
        "skeleton"            \
    )

#define ENTITY_MUSHROOM(x, y) \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            0,                \
            SPRITE_MUSHROOM,  \
            3,                \
            false,            \
            "s_mushroom" \
        ),                    \
        3.0f,                 \
        100,                  \
        0.3,                  \
        0.4,                  \
        6,                    \
        1.0f,                 \
        MoveSmoothAStar,      \
        "mushroom"            \
    )

#define ENTITY_EYE(x, y)      \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            0,                \
            SPRITE_EYE,       \
            3,                \
            false,            \
            "s_eye" \
            ),                \
        5.0f,                 \
        100,                  \
        0.4,                  \
        0.6,                  \
        5,                    \
        1.0f,                 \
        MoveSmoothAStar,      \
        "eye"                 \
    )

#define SPRITE_COLUMN(x, y)   \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_MIDDLE,        \
        SPRITE_COLUMN,        \
        2,                    \
        true,                 \
        "s_column" \
    )
#define SPRITE_LAMP(x, y)     \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_UP,            \
        SPRITE_LAMP,          \
        2,                    \
        false,                \
        "s_lamp" \
    )
#define SPRITE_BARREL(x, y)   \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_DOWN,          \
        SPRITE_BARREL,        \
        2,                    \
        true,                 \
        "s_barrel" \
    )

#define ITEM_KEY(x, y)        \
    ItemNew(                  \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            -7,               \
            SPRITE_KEY,       \
            0.5,              \
            false,            \
            "s_key" \
        ),                    \
        "key"                 \
    )

#define ITEM_GLASSES(x, y)    \
    ItemNew(                  \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            -7,               \
            SPRITE_GLASSES,   \
            0.5,              \
            false,            \
            "s_glasses" \
        ),                    \
        "glasses"             \
    )

#define ITEM_SWORD(x, y)    \
    ItemNew(                  \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            -7,               \
            SPRITE_SWORD,   \
            0.5,              \
            false,            \
            "s_sword" \
        ),                    \
        "sword"             \
    )

#endif // OBJECTS_H
