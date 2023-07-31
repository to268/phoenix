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
#ifndef __KERNEL_H_
#define __KERNEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/types.h>
#include <phoenix/boot.h>

#define KERN_VERBOSE 4
#define KERN_INFO    3
#define KERN_WARN    2
#define KERN_ERROR   1
#define KERN_FATAL   0

#define INT_LENGTH 8

/* Printing and Formating */
MAYBE_UNUSED NONNULL int printk(u8 severity, const char* restrict format, ...);
MAYBE_UNUSED int putchar(int ic, u8 severity);
MAYBE_UNUSED NONNULL int puts(u8 severity, const char* string);
NODISCARD uptr convert_to_mb(uintptr_t nb_bytes);
MAYBE_UNUSED NONNULL int convert_int_to_char(int number, int base, char* buff);

/* Macros for printk */
#define verbose(...) printk(KERN_VERBOSE, __VA_ARGS__)
#define info(...)    printk(KERN_INFO, __VA_ARGS__)
#define warn(...)    printk(KERN_WARN, __VA_ARGS__)
#define error(...)   printk(KERN_ERROR, __VA_ARGS__)

/* Basic Functions */
NODISCARD int abs(int x);
NODISCARD NONNULL int atoi(const char* nptr);
MAYBE_UNUSED NONNULL RETURNS_NONNULL char* itoa(int value, char* buffer,
                                                int base);
NORETURN void panic(const char* msg);

/* Kernel entry points */
void init(void);
void kernel_main(void);

#ifdef __cplusplus
}
#endif

#endif /*__KERNEL_H_*/
