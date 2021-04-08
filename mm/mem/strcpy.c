#include <phoenix/mem.h>
#include <stddef.h>

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);

char* strcpy(char* dest, const char* src) {
    size_t i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return dest;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i = 0;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}
