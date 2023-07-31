/*
 * Copyright © 2023 Guillot Tony <tony.guillot@protonmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <phoenix/kernel.h>
#include <phoenix/mem.h>

MAYBE_UNUSED NONNULL RETURNS_NONNULL char* itoa(int value, char* buffer,
                                                int base) {
    /* Check if the base is valid */
    if (base < 2 || base > 32) {
        *buffer = '\0';
        return buffer;
    }

    /* Set minus for negative decimals */
    if (value < 0 && base == 10) {
        memmove(buffer, "-", 1);
        buffer++;
    }

    /* Get absolute value */
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

    /* Check if number is 0 */
    if (i == 0)
        buffer[i++] = '0';

    /* Add NULL terminator */
    buffer[i] = '\0';

    /* reverse the string */
    int len = strlen(buffer);
    for (int i = 0; i < len / 2; i++) {
        char temp = buffer[i];
        buffer[i] = buffer[len - 1 - i];
        buffer[len - 1 - i] = temp;
    }
    return buffer;
}
