#include "version.h"
#define CLI_IMPLEMENTATION
#include "extern/cli.h"
#include "cli.h"
#include <stdlib.h>
#include <stdio.h>


void Usage()
{
    printf("Usage: %s\n", USAGE);
}

Input ParseCliArgs(int argc, char** argv)
{
    if(argc == 1) {
        Usage();
        exit(1);
    }

    Input result = {
        .rows = 10,
        .cols = 10,
        .scale = 60,
        .output = NULL,
        .loaded = NULL
    };
    cli_args_t args = cli_args_make(
        cli_arg_new('h', "help", "Prints this message", no_argument),
        cli_arg_new('v', "version", "Prints the version of the program", no_argument),
        cli_arg_new('r', "rows", "Specify the level's rows", required_argument),
        cli_arg_new('c', "cols", "Specify the level's columns", required_argument),
        cli_arg_new('s', "scale", "Specify the scale of each square (defaults to 60)", required_argument),
        cli_arg_new('o', "output", "Specify the output path", required_argument),
        cli_arg_new('L', "load", "Load a level", required_argument),
        NULL
    );
    int opt;
    LOOP_ARGS(opt, args) {
        switch (opt) {
            case 'h':
                cli_help(args, USAGE, "Written by KDesp73 (Konstantinos Despoinidis)");
                cli_args_free(&args);
                exit(0);
            case 'v':
                printf("%s v%s\n", PROGRAM_NAME, VERSION);
                cli_args_free(&args);
                exit(0);
            case 'r':
                result.rows = atoi(optarg);
                break;
            case 'c':
                result.cols = atoi(optarg);
                break;
            case 's':
                result.scale = atoi(optarg);
                break;
            case 'o':
                result.output = strdup(optarg);
                break;
            case 'L':
                result.loaded = strdup(optarg);
                break;
            default:
                cli_args_free(&args);
                exit(1);
        }
    }
    cli_args_free(&args);
    return result;
}

