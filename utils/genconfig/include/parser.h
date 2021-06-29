#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define MAX_LENGTH      4096
#define MAX_SECTIONS    128
#define DATA_FILE       "data/data.kcp"

/* Tokens */
#define NAME_TKN        " ->"
#define DESCRIPTION_TKN "description: "
#define ARCH_TKN        "arch: "
#define FILES_TKN       "files: "
#define DIRS_TKN        "dirs: "
#define DEFINES_TKN     "defines: "
#define PROPERTY_TKN    "property: "
#define DEPENDS_TKN     "depends: "
#define CONFLICTS_TKN   "conflicts: "
#define SEPARATOR_TKN   ", "
#define DEFAULT_TKN     "#"
#define OR_TKN          " | "

/* Variables */
#define ARCH_VAR        "$ARCH"

struct config_section {
    char name[MAX_LENGTH];
    char files[MAX_LENGTH];
    char dirs[MAX_LENGTH];
    char defines[MAX_LENGTH];
    char properties[MAX_LENGTH];
};

struct config {
    int sections_len;
    int conflicts_len;
    char arch[MAX_LENGTH];
    struct config_section* sections[MAX_SECTIONS];
    struct config_section* conflicts[MAX_SECTIONS];
};

enum propety_type {
    NAME,
    DESCRIPTION,
    ARCH,
    FILES,
    DIRS,
    DEFINES,
    PROPERTY,
    DEPENDS,
    CONFLICTS,
    UNKNOWN,
};

struct config* config_alloc(void);
void config_free(struct config* config);
FILE* open_data_file(void);
void close_data_file(FILE* fd);
void parse_section(struct config* config, char* name, FILE* fd);
void set_arch(struct config* config, char* arch);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
