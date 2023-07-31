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
#ifndef _PIT_H_
#define _PIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

#define PIT_IRQ  0
#define PIT_FREQ 1193180

void pit_init(u32 hz);
void pit_handler(void);
void pit_sleep(u64 msec);
NODISCARD u64 pit_get_ticks(void);

#ifdef __cplusplus
}
#endif

#endif /* _PIT_H_*/
