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
#include <phoenix/mem.h>
#include <phoenix/types.h>
#include <stddef.h>

DIAGNOSE_AS_BUILTIN(__builtin_strcat, 1, 2)
MAYBE_UNUSED NONNULL RETURNS_NONNULL char* strcat(char* dest, const char* src) {
    size_t dest_len = strlen(dest);
    size_t i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';

    return dest;
}

DIAGNOSE_AS_BUILTIN(__builtin_strncat, 1, 2, 3)
MAYBE_UNUSED NONNULL RETURNS_NONNULL char* strncat(char* dest, const char* src,
                                                   size_t n) {
    size_t dest_len = strlen(dest);
    size_t i = 0;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';

    return dest;
}
