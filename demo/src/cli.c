#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#define CLI_IMPLEMENTATION
#include "io/cli.h"
#include "cli.h"

CliArgs ParseCliArgs(int argc, char** argv)
{
    CliArgs res = {0};

    cli_args_t args = cli_args_make(
        cli_arg_new('h', "help", "Prints this message", no_argument),
        cli_arg_new('v', "version", "Prints the engine version", no_argument),
        cli_arg_new('F', "fullscreen", "Start the game in fullscreen", no_argument),
        cli_arg_new('l', "level", "Specify the level file to load", required_argument),
#ifdef DEBUG
        cli_arg_new('D', "debug", "Start in debug mode", no_argument),
#endif
        NULL
    );

    int opt;
    LOOP_ARGS(opt, args) {
        switch (opt) {
            case 'h':
                cli_help(args, "eidolon [-F] [-l <index>] [-h | -v]", "Written by KDesp73 (Konstantinos Despoinidis)");
                exit(0);
            case 'v':
                printf("castor v%s\n", CASTOR_VERSION);
                exit(0);
            case 'F':
                res.fullscreen = true;
                break;
            case 'l':
                res.level = strdup(optarg);
                break;
#ifdef DEBUG
            case 'D':
                res.debug = true;
                break;
#endif
            default:
                exit(1);
        } 
    }
    cli_args_free(&args);

    return res;
}

