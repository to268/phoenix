#include <phoenix/mem.h>
#include <stddef.h>

char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);

char* strcat(char* dest, const char* src) {
    size_t dest_len = strlen(dest);
    size_t i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';

    return dest;
}

char* strncat(char* dest, const char* src, size_t n) {
    size_t dest_len = strlen(dest);
    size_t i = 0;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';

    return dest;
}
