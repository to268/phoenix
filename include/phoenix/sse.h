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
#ifndef _SSE_H_
#define _SSE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ARCH_X86_64)

void sse_init(void);
void sse_save_state(void);
void sse_restore_state(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _SSE_H_ */
