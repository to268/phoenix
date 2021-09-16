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
#include <phoenix/kernel.h>
#include <phoenix/structs/bitmap.h>

#define BYTE_INDEX(bit) (bit / 8)
#define BIT_INDEX(bit)  (bit % 8)

void bitmap_set(bitmap_t* bitmap, u64 bit);
void bitmap_clear(bitmap_t* bitmap, u64 bit);
bool bitmap_test(bitmap_t* bitmap, u64 bit);

void bitmap_set(bitmap_t* bitmap, u64 bit)
{
    if (bit > bitmap->size)
        return;

    u64 byte_index = BYTE_INDEX(bit);
    u8 bit_index = BIT_INDEX(bit);

    bitmap->bitmap[byte_index] |= (1 << bit_index);
}

void bitmap_clear(bitmap_t* bitmap, u64 bit)
{
    if (bit > bitmap->size)
        return;

    u64 byte_index = BYTE_INDEX(bit);
    u8 bit_index = BIT_INDEX(bit);

    bitmap->bitmap[byte_index] &= ~(1 << bit_index);
}

bool bitmap_test(bitmap_t* bitmap, u64 bit)
{
    if (bit > bitmap->size)
        return NULL;

    u64 byte_index = BYTE_INDEX(bit);
    u8 bit_index = BIT_INDEX(bit);

    return bitmap->bitmap[byte_index] & (1 << bit_index);
}
