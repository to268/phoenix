#include <phoenix/kernel.h>
#include <phoenix/mem.h>

char* itoa(int value, char* buffer, int base);

char* itoa(int value, char* buffer, int base) {
    // Check if the base is valid
    if (base < 2 || base > 32) {
        *buffer = '\0';
        return buffer;
    }

    // Get absolute value
    int n = abs(value);

    int i = 0;
    while (n) {
        int r = n % base;

        if (r >= 10)
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;

        n = n / base;
    }

    // Check if number is 0
    if (i == 0)
        buffer[i++] = '0';

    // Set minus for negative decimals
    if (value < 0 && base == 10)
        memmove(buffer, "-", 1);

    // Add NULL terminator
    buffer[i] = '\0';

    // reverse the string
    int len = strlen(buffer);
    for (int i = 0; i < len / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[len-1-i];
        buffer[len-1-i] = temp;
    }
    return buffer;
}
