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
#include <phoenix/types.h>
#include <stddef.h>

void* memset(void* bufptr, int value, size_t size);

DIAGNOSE_AS_BUILTIN(__builtin_memset, 1, 2, 3)
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
MAYBE_UNUSED NONNULL RETURNS_NONNULL void* memset(void* bufptr, int value,
                                                  size_t size) {
    unsigned char* buf = (unsigned char*)bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (unsigned char)value;
    return bufptr;
}
