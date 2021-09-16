/*
 * Copyright © 2021 Guillot Tony <tony.guillot@protonmail.com>
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
#ifndef _BITMAP_H_
#define _BITMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/types.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct bitmap {
    u8* bitmap;
    size_t size;
    uptr* base;
} bitmap_t;

void bitmap_set(bitmap_t* bitmap, u64 bit);
void bitmap_clear(bitmap_t* bitmap, u64 bit);
bool bitmap_test(bitmap_t* bitmap, u64 bit);

#endif /* _BITMAP_H_ */
