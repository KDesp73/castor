#ifndef GAME_CLI_H
#define GAME_CLI_H

#include <stdbool.h>
typedef struct {
    bool fullscreen;
    int level;
} CliArgs;

CliArgs ParseCliArgs(int argc, char** argv);

#endif // GAME_CLI_H
