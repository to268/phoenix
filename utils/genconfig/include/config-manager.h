#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Basic actions */
int create_config(char* arch, char* path);
int check_config(char* arch, char* path);
int upgrade_config(char* arch, char* path);

/* Makefile specific actions */
int get_asm_files(char* path);
int get_c_files(char* path);
int get_defines(char* path);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_MANAGER_H */
