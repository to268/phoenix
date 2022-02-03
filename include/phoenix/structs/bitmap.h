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
#ifndef _BITMAP_H_
#define _BITMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    u8* bitmap;
    size_t size;
    uptr* base;
} Bitmap;

void bitmap_set(Bitmap* bitmap, u64 bit);
void bitmap_clear(Bitmap* bitmap, u64 bit);
bool bitmap_check(Bitmap* bitmap, u64 bit);

#endif /* _BITMAP_H_ */
