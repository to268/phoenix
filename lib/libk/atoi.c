#include <phoenix/kernel.h>

int atoi(const char* nptr);

int atoi(const char* nptr) {
    int res = 0;

    for (int i = 0; nptr[i] != '\0'; i++)
        res = res * 10 + nptr[i] - '0';

    return res;
}
