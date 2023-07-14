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
#ifndef __MEM_H_
#define __MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

int memcmp(const void* aptr, const void* bptr, size_t size);
void* memcpy(void* __restrict dstptr, const void* __restrict srcptr,
             size_t size);
void* memmove(void* dstptr, const void* srcptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif

#endif /*__MEM_H_*/
