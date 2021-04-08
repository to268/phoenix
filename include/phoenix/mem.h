#ifndef __MEM_H_
#define __MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

int memcmp(const void* aptr, const void* bptr, size_t size);
void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size);
void* memmove(void* dstptr, const void* srcptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif

#endif
