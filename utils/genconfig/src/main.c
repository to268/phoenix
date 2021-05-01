#include <stdio.h>
#include <string.h>
#include "config-manager.h"

#define VERSION "v0.1-alpha"

int args_handler(int argc, char **argv);
void args_error(void);
void print_help(void);
void print_version(void);

int main(int argc, char **argv) {
    return args_handler(argc, argv);
}

int args_handler(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        args_error();
        return 1;
    }

    if (!strcmp(argv[1], "create")) {
        return create_config(argv[2]);

    } else if (!strcmp(argv[1], "check")) {
        return check_config(argv[2]);

    } else if (!strcmp(argv[1], "upgrade")) {
        return upgrade_config(argv[2]);

    } else if (!strcmp(argv[1], "help")) {
        print_help();
        return 0;

    } else if (!strcmp(argv[1], "version")) {
        print_version();
        return 0;

    } else if (!strcmp(argv[1], "asm-files")) {
        return get_asm_files(argv[2]);

    } else if (!strcmp(argv[1], "c-files")) {
        return get_c_files(argv[2]);
    }

    args_error();

    return 1;
}

void args_error(void) {
    printf("Usage: genconfig [ACTION] [FILE]\n");
    printf("Try 'genconfig help' for more information\n");
}

void print_help(void) {
    printf("genconfig [ACTION] [FILE]\n");
    printf("Avariable actions: create, check, upgrade, help, version\n");
    printf("File: config file path\n");
}

void print_version(void) {
    printf("genconfig ");
    printf("%s\n", VERSION);
}
