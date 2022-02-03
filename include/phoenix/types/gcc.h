
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
#ifndef _GCC_H_
#define _GCC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LIKELY(x)           __builtin_expect((x), 1)
#define UNLIKELY(x)         __builtin_expect((x), 0)
#define ALIGNED(x)          __attribute__((aligned(x)))
#define SECTION(x)          __attribute__((section(x), used))
#define PACKED              __attribute__((packed))
#define NORETURN            __attribute__((noreturn))
#define RANDOMIZE_LAYOUT    __attribute__((randomize_layout))

#endif /* _GCC_H_ */
