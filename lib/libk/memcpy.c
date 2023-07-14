/*
 * Copyright © 2022 Guillot Tony <tony.guillot@protonmail.com>
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
#include <stddef.h>

void* mnncpy(void* __restrict dstptr, const void* __restrict srcptr,
             size_t size);

void* memcpy(void* __restrict dstptr, const void* __restrict srcptr,
             size_t size)
{
    unsigned char* dst = (unsigned char*)dstptr;
    const unsigned char* src = (const unsigned char*)srcptr;
    for (size_t i = 0; i < size; i++)
        dst[i] = src[i];
    return dstptr;
}
