#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

struct config* config_alloc(void);
void config_free(struct config* config);
FILE* open_data_file(void);
void close_data_file(FILE* fd);
void parse_section(struct config* config, char* name, FILE* fd);
enum propety_type parse_propety(struct config_section* section, char* buff);
void strip_property(struct config_section* section, char* src, enum propety_type type);
void set_arch(struct config* config, char* arch);

struct config* config_alloc(void) {
    return malloc(sizeof(struct config));
}

void config_free(struct config* config) {
    /* Free all sections */
    for (int i = 0; i < config->sections_len; i++)
        free(config->sections[i]);

    /* Free all conflicts */
    for (int i = 0; i < config->conflicts_len; i++)
        free(config->conflicts[i]);

    /* Free config */
    free(config);
}

FILE* open_data_file(void) {
    FILE* fd = fopen(DATA_FILE, "r");
    if (fd == NULL) {
        printf("Error cannot open %s\n", DATA_FILE);
        exit(EXIT_FAILURE);
    }
    return fd;
}

void close_data_file(FILE* fd) {
    fclose(fd);
}

void parse_section(struct config* config, char* name, FILE* fd) {
    struct config_section* section = malloc(sizeof(struct config_section));

    if (section == NULL) {
        printf("Error malloc failure\n");
        exit(EXIT_FAILURE);
    }

    /* Find section */
    char buff[MAX_LENGTH];
    enum propety_type type = UNKNOWN;

    /* Get name first */
    int found = 0;
    while (feof(fd) == 0) {
        bzero(buff, MAX_LENGTH);
        fgets(buff, MAX_LENGTH, fd);
        type = parse_propety(section, buff);

        /* Found the good section */
        if (type == NAME && strstr(buff, name) != NULL) {
            found = 1;
            break;
        }
    }

    /* If nothing is found */
    if (!found) {
        printf("Base section not found !\n");
        config_free(config);
        exit(EXIT_FAILURE);
    }

    /* Add section */
    config->sections[config->sections_len] = section;
    config->sections_len++;

    strip_property(section, buff, NAME);

    /* Process others properties */
    rewind(fd);
    while (feof(fd) == 0) {
        bzero(buff, MAX_LENGTH);
        fgets(buff, MAX_LENGTH, fd);
        type = parse_propety(section, buff);
        strip_property(section, buff, ARCH);

        switch (type) {
            case ARCH:
                if (strcmp(buff, config->arch)) {
                    printf("Error: %s section does not support %s arch\n",
                        section->name, config->arch);

                    config_free(config);
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                break;
        }
    }

}

enum propety_type parse_propety(struct config_section* section, char* buff) {
    /* Name */
    if (strstr(buff, NAME_TKN) != NULL) {
        return NAME;
    }

    /* Description */
    if (strstr(buff, DESCRIPTION_TKN) != NULL) {
        return DESCRIPTION;
    }

    /* Arch */
    if (strstr(buff, ARCH_TKN) != NULL) {
        return ARCH;
    }

    /* Files */
    if (strstr(buff, FILES_TKN) != NULL) {
        return FILES;
    }

    /* Dirs */
    if (strstr(buff, DIRS_TKN) != NULL) {
        return DIRS;
    }

    /* Defines */
    if (strstr(buff, DEFINES_TKN) != NULL) {
        return DEFINES;
    }

    /* Property */
    if (strstr(buff, PROPERTY_TKN) != NULL) {
        return PROPERTY;
    }

    /* Depends */
    if (strstr(buff, DEPENDS_TKN) != NULL) {
        return DEPENDS;
    }

    /* Conflicts */
    if (strstr(buff, CONFLICTS_TKN) != NULL) {
        return CONFLICTS;
    }

    return UNKNOWN;
}

void strip_property(struct config_section* section, char* src, enum propety_type type) {
    char* tkn;
    switch (type) {

        case NAME:
            tkn = strstr(src, NAME_TKN);
            strncpy(section->name, src, (tkn - src));
            strtok(src, "\n");
            break;

        case ARCH:
            tkn = strstr(src, ARCH_TKN);
            strcpy(src, src + strlen(ARCH_TKN));
            strtok(src, "\n");
            break;

        case FILES:
            tkn = strstr(src, FILES_TKN);
            strcpy(section->files, src + strlen(FILES_TKN));
            strtok(src, "\n");
            break;

        case DIRS:
            tkn = strstr(src, DIRS_TKN);
            strcpy(section->dirs, src + strlen(DIRS_TKN));
            strtok(src, "\n");
            break;

        case DEFINES:
            tkn = strstr(src, DEFINES_TKN);
            strcpy(section->defines, src + strlen(DEFINES_TKN));
            strtok(src, "\n");
            break;

        case PROPERTY:
            tkn = strstr(src, PROPERTY_TKN);
            strcpy(section->properties, src + strlen(PROPERTY_TKN));
            strtok(src, "\n");
            break;

        case DEPENDS:
            tkn = strstr(src, DEPENDS_TKN);
            strcpy(src, tkn);
            strtok(src, "\n");
            break;

        case CONFLICTS:
            tkn = strstr(src, CONFLICTS_TKN);
            strcpy(src, tkn);
            strtok(src, "\n");
            break;

        default:
            break;
    }
}

void set_arch(struct config* config, char* arch) {
    strcpy(config->arch, arch);
}
