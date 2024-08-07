/*
 * Copyright © 2024 Guillot Tony <tony.guillot@protonmail.com>
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
#include <phoenix/types.h>
#include <stddef.h>

DIAGNOSE_AS_BUILTIN(__builtin_strcpy, 1, 2)
MAYBE_UNUSED NONNULL RETURNS_NONNULL char* strcpy(char* dest, const char* src) {
    size_t i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
    return dest;
}

DIAGNOSE_AS_BUILTIN(__builtin_strncpy, 1, 2, 3)
MAYBE_UNUSED NONNULL RETURNS_NONNULL char* strncpy(char* dest, const char* src,
                                                   size_t n) {
    size_t i = 0;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}
