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

DIAGNOSE_AS_BUILTIN(__builtin_strcmp, 1, 2)
NODISCARD NONNULL int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2))
        s1++, s2++;

    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
