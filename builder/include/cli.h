#ifndef LVL_CLI_H
#define LVL_CLI_H

#include <stddef.h>
#define PROGRAM_NAME "lvl"
#define USAGE "lvl -r <ROWS> -c <COLS> [-s <SCALE>] [-o <PATH>]"

void Usage();

typedef struct {
    size_t rows, cols, scale;
    char* output, *loaded;
} Input;

Input ParseCliArgs(int argc, char** argv);

#endif // LVL_CLI_H
