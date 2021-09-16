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
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/io.h>
#include <stddef.h>

static int serial_port_check(const u16 serial_port);
static u8 serial_initialized = 0;

/* Initialize serial connexion */
int serial_init(const u16 serial_port, uint16_t baud_divisor)
{
    /* Check is the serial port is supported */
    if (serial_port_check(serial_port)) return 1;

    outb(serial_port + 1, 0x00);                         /* Disable all interrupts */
    outb(serial_port + 3, 0x80);                         /* Enable DLAB (set baud rate divisor) */
    outb(serial_port + 0, baud_divisor & 0xff);          /* Set divisor (low byte) */
    outb(serial_port + 1, (baud_divisor & 0xff00) >> 8); /* Set divisor (high byte) */
    outb(serial_port + 3, 0x03);                         /* 8 bits, no parity, one stop bit */
    outb(serial_port + 2, 0xc7);                         /* Enable FIFO */
    outb(serial_port + 4, 0x0b);                         /* IRQs enabled, RTS/DSR set */
    outb(serial_port + 4, 0x1e);                         /* Set in loopback mode, test the serial chip */
    outb(serial_port + 0, 0xae);                         /* Test serial chip */

    /* Check if serial output is the same */
    if (inb(serial_port + 0) != 0xae) {
        return 1;
    }

    /* If serial is working, set it in normal operation mode */
    outb(serial_port + 4, 0x0f);
    serial_initialized = 1;
    return 0;
}

/* Check if a serial port is valid */
static int serial_port_check(const u16 serial_port)
{
    if (serial_port == SERIAL_COM1 || serial_port == SERIAL_COM2 ||
        serial_port == SERIAL_COM3 || serial_port == SERIAL_COM4) {
        return 0;
    }
    return 1;
}

/* Check if we can receive data */
int serial_received(const u16 serial_port)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return -1;

    return inb(serial_port + 5) & 1;
}

/* Read data thought serial */
char serial_read(const u16 serial_port)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return -1;

    while (serial_received(serial_port) == 0);

    return inb(serial_port);
}

/* Read a string thought serial */
void serial_readstring(const u16 serial_port, char* buff, size_t size)
{
    size_t i = 0;
    char c = serial_read(serial_port);
    while (c != -1 || c != '\n' || c != '\r') {
    if (i >= (size - 1))
        break;

    buff[i] = c;
    c = serial_read(serial_port);
    i++;
    }

    /* Add NULL terminator */
    *buff = '\0';
}

/* Check the end of the received data */
static int serial_is_transmit_empty(const u16 serial_port)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return -1;

    return inb(serial_port + 5) & 0x20;
}

/* Write a char thought serial */
void serial_write(const u16 serial_port, const char c)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return;

    while (serial_is_transmit_empty(serial_port) == 0);

    outb(serial_port, c);
}

/* Write a string thought serial */
void serial_writestring(const u16 serial_port, const char* string)
{
    while (*string) {
        serial_write(serial_port, *(string++));
    }
}
