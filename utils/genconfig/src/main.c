#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config-manager.h"

#define VERSION "v0.1-dev"

int args_handler(int argc, char **argv);
int args_validate(int argc);
void args_error(void);
void print_help(void);
void print_version(void);

int main(int argc, char **argv) {
    return args_handler(argc, argv);
}

int args_handler(int argc, char **argv) {
    if (argc < 2 || argc > 4) {
        args_error();
        return EXIT_FAILURE;
    }

    if (!strcmp(argv[1], "create")) {
        if (!args_validate(argc))
            return create_config(argv[2], argv[3]);

    } else if (!strcmp(argv[1], "check")) {
        if (!args_validate(argc))
            return check_config(argv[2], argv[3]);

    } else if (!strcmp(argv[1], "upgrade")) {
        if (!args_validate(argc))
            return upgrade_config(argv[2], argv[3]);

    } else if (!strcmp(argv[1], "help")) {
        print_help();
        return EXIT_SUCCESS;

    } else if (!strcmp(argv[1], "version")) {
        print_version();
        return EXIT_SUCCESS;

    /* Makefile specific actions */
    } else if (!strcmp(argv[1], "asm-files")) {
        return get_asm_files(argv[2]);

    } else if (!strcmp(argv[1], "c-files")) {
        return get_c_files(argv[2]);

    } else if (!strcmp(argv[1], "defines")) {
        return get_defines(argv[2]);
    }

    args_error();

    return EXIT_FAILURE;
}

int args_validate(int argc) {
    if (argc != 4) {
        args_error();
        return EXIT_FAILURE;
    }
    return 0;
}

void args_error(void) {
    printf("Usage: genconfig [ACTION] [ARCH] [FILE]\n");
    printf("Try 'genconfig help' for more information\n");
}

void print_help(void) {
    printf("genconfig [ACTION] [ARCH] [FILE]\n");
    printf("Avariable actions: create, check, upgrade, help, version\n");
    printf("Arch: target architecture\n");
    printf("File: config file path\n");
}

void print_version(void) {
    printf("genconfig ");
    printf("%s\n", VERSION);
}
