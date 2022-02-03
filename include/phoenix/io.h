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
#ifndef _IO_H_
#define _IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

static inline void cli(void);
static inline void sti(void);
static inline u8 inb(u16 port);
static inline u16 inw(uint16_t port);
static inline u8 inb_p(u16 port);
static inline u16 inw_p(uint16_t port);
static inline void outb(u16 port, u8 val);
static inline void outw(u16 port, uint16_t val);
static inline void outb_p(u16 port, u8 val);
static inline void outw_p(u16 port, uint16_t val);
static inline void io_wait(void);

static inline void cli(void)
{
    asm volatile("cli" ::);
}

static inline void sti(void)
{
    asm volatile("sti" ::);
}

static inline u8 inb(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u16 inw(uint16_t port)
{
    u16 ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u8 inb_p(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0\noutb %%al, $0x80" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u16 inw_p(uint16_t port)
{
    u16 ret;
    asm volatile("inw %1, %0\noutb %%al, $0x80" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(u16 port, u8 val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(u16 port, uint16_t val)
{
    asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outb_p(u16 port, u8 val)
{
    asm volatile("outb %0, %1\noutb %%al,$0x80" : : "a"(val), "Nd"(port));
}

static inline void outw_p(u16 port, uint16_t val)
{
    asm volatile("outw %0, %1\noutb %%al,$0x80" : : "a"(val), "Nd"(port));
}


static inline void io_wait(void)
{
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

#ifdef __cplusplus
}
#endif

#endif /* _IO_H_ */
