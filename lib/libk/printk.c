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
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/vga.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>

#define EOF (-1)

static int print(const char* data, size_t length, u8 severity);
inline uptr convert_to_mb(uintptr_t nb_bytes);
int convert_int_to_char(int number, int base, char* buff);
int printk(u8 severity, const char* restrict format, ...);

static int print(const char* data, size_t length, u8 severity) {
    const unsigned char* bytes = (const unsigned char*)data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i], severity) == EOF)
            return 0;
    return 1;
}

inline uptr convert_to_mb(uintptr_t nb_bytes) { return nb_bytes / 1024 / 1024; }

int convert_int_to_char(int number, int base, char* buff) {
    itoa(number, buff, base);
    int len = strlen(buff);
    return len;
}

int printk(u8 severity, const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                goto error;
            }
            if (!print(format, amount, severity))
                goto error;
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        switch (*format) {
        case 'c':
        case 'C':
            format++;
            char c = (char)va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                goto error;
            }
            if (!print(&c, sizeof(c), severity))
                goto error;
            written++;
            break;

        case 's':
        case 'S':
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t str_len = strlen(str);
            if (maxrem < str_len) {
                goto error;
            }
            if (!print(str, str_len, severity))
                goto error;
            written += str_len;
            break;

        case 'b':
        case 'B':
            format++;
            int number = va_arg(parameters, int);
            char buff[INT_LENGTH];
            /* Convert char to binary */
            int len = convert_int_to_char(number, 2, buff);
            if (!print(buff, len, severity))
                goto error;
            written += len;
            break;

        case 'o':
        case 'O':
            format++;
            number = va_arg(parameters, int);
            /* Convert char to octal base */
            len = convert_int_to_char(number, 8, buff);
            if (!print(buff, len, severity))
                goto error;
            written += len;
            break;

        case 'd':
        case 'D':
            format++;
            number = va_arg(parameters, int);
            /* Convert char to decimal */
            len = convert_int_to_char(number, 10, buff);
            if (!print(buff, len, severity))
                goto error;
            written += len;
            break;

        case 'p':
        case 'P':
            format++;
            uptr ptr = va_arg(parameters, uintptr_t);
            /* Convert char to hexadecimal */
            len = convert_int_to_char(ptr, 16, buff);
            if (!print(buff, len, severity))
                goto error;
            written += len;
            break;

        case 'x':
        case 'X':
            format++;
            number = va_arg(parameters, int);
            /* Convert pointer to hexadecimal */
            len = convert_int_to_char(number, 16, buff);
            if (!print(buff, len, severity))
                goto error;
            written += len;
            break;

        default:
            format = format_begun_at;
            str_len = strlen(format);
            if (maxrem < str_len) {
                goto error;
            }
            if (!print(format, str_len, severity))
                goto error;
            written += str_len;
            format += str_len;
            break;
        }
    }

    va_end(parameters);
    return written;

error:
    va_end(parameters);
    return -1;
}
