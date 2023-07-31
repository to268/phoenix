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
#ifndef _IO_H_
#define _IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

ALWAYS_INLINE static void cli(void);
ALWAYS_INLINE static void sti(void);
ALWAYS_INLINE static u8 inb(u16 port);
ALWAYS_INLINE static inline u16 inw(u16 port);
ALWAYS_INLINE static inline u8 inb_p(u16 port);
ALWAYS_INLINE static inline u16 inw_p(u16 port);
ALWAYS_INLINE static inline void outb(u16 port, u8 val);
ALWAYS_INLINE static inline void outw(u16 port, u16 val);
ALWAYS_INLINE static inline void outb_p(u16 port, u8 val);
ALWAYS_INLINE static inline void outw_p(u16 port, u16 val);
ALWAYS_INLINE static inline void io_wait(void);

ALWAYS_INLINE static inline void cli(void) { __asm__ volatile("cli" ::); }

ALWAYS_INLINE static inline void sti(void) { __asm__ volatile("sti" ::); }

ALWAYS_INLINE static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

ALWAYS_INLINE static inline u16 inw(u16 port) {
    u16 ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

ALWAYS_INLINE static inline u8 inb_p(u16 port) {
    u8 ret;
    __asm__ volatile("inb %1, %0\noutb %%al, $0x80" : "=a"(ret) : "Nd"(port));
    return ret;
}

ALWAYS_INLINE static inline u16 inw_p(u16 port) {
    u16 ret;
    __asm__ volatile("inw %1, %0\noutb %%al, $0x80" : "=a"(ret) : "Nd"(port));
    return ret;
}

ALWAYS_INLINE static inline void outb(u16 port, u8 val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

ALWAYS_INLINE static inline void outw(u16 port, u16 val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

ALWAYS_INLINE static inline void outb_p(u16 port, u8 val) {
    __asm__ volatile("outb %0, %1\noutb %%al,$0x80" : : "a"(val), "Nd"(port));
}

ALWAYS_INLINE static inline void outw_p(u16 port, u16 val) {
    __asm__ volatile("outw %0, %1\noutb %%al,$0x80" : : "a"(val), "Nd"(port));
}

ALWAYS_INLINE static inline void io_wait(void) {
    __asm__ volatile("outb %%al, $0x80" : : "a"(0));
}

#ifdef __cplusplus
}
#endif

#endif /* _IO_H_ */
