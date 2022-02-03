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
#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>
#include <stddef.h>

/* Serial Ports */
#define SERIAL_COM1 0x3f8
#define SERIAL_COM2 0x2f8
#define SERIAL_COM3 0x3e8
#define SERIAL_COM4 0x2e8

#define debug(string) serial_writestring(SERIAL_COM1, string)

/* Serial Functions */
int serial_init(const u16 serial_port, uint16_t baud_divisor);
int serial_received(const u16 serial_port);
char serial_read(const u16 serial_port);
void serial_readstring(const u16 serial_port, char* buff, size_t size);
void serial_write(const u16 serial_port, const char c);
void serial_writestring(const u16 serial_port, const char* string);

#ifdef __cplusplus
}
#endif

#endif /*_SERIAL_H_*/
