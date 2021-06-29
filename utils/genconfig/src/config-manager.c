#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "config-manager.h"


int create_config(char* arch, char* path) {
    printf("Arch: %s\n", arch);
    printf("Config path: %s\n", path);

    /* Test config creation process */
    struct config* config = config_alloc();
    set_arch(config, arch);
    FILE* fd = open_data_file();
    parse_section(config, arch, fd);
    close_data_file(fd);
    config_free(config);

    return 0;
}

int check_config(char* arch, char* path) {
    printf("Arch: %s\n", arch);
    printf("Config path: %s\n", path);
    return 0;
}

int upgrade_config(char* arch, char* path) {
    printf("Arch: %s\n", arch);
    printf("Config path: %s\n", path);
    return 0;
}

int get_asm_files(char* path) {
    printf("Config path: %s\n", path);
    return 0;
}

int get_c_files(char* path) {
    printf("Config path: %s\n", path);
    return 0;
}

int get_defines(char* path) {
    printf("Config path: %s\n", path);
    return 0;
}
