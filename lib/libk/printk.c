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
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/vga.h>
#include <stddef.h>
#include <stdarg.h>

#define EOF (-1)

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
MAYBE_UNUSED NONNULL static int print(const char* data, size_t length,
                                      u8 severity) {
    auto bytes = (const unsigned char*)data;

    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i], severity) == EOF)
            return 0;

    return 1;
}

NODISCARD ALWAYS_INLINE uptr convert_to_mb(uintptr_t nb_bytes) {
    return nb_bytes / 1024 / 1024;
}

MAYBE_UNUSED NONNULL int convert_int_to_char(int number, int base, char* buff) {
    itoa(number, buff, base);
    auto len = strlen(buff);
    return (int)len;
}

DIAGNOSE_AS_BUILTIN(__builtin_printf, 2)
MAYBE_UNUSED NONNULL int printk(u8 severity, const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    auto written = 0;

    while (*format != '\0') {
        size_t maxrem = __INT_MAX__ - written;

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
            written += (int)amount;
            continue;
        }

        const char* format_begun_at = format++;

        switch (*format) {
        case 'c':
        case 'C':
            format++;

            auto c = (char)va_arg(parameters, int /* char promotes to int */);

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

            auto str = va_arg(parameters, const char*);
            auto str_len = strlen(str);

            if (maxrem < str_len) {
                goto error;
            }

            if (!print(str, str_len, severity))
                goto error;

            written += (int)str_len;
            break;

        case 'b':
        case 'B':
            format++;
            auto number = va_arg(parameters, int);
            char buff[INT_LENGTH];

            /* Convert char to binary */
            auto len = convert_int_to_char(number, 2, buff);
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
            auto ptr = va_arg(parameters, uintptr_t);

            /* Convert char to hexadecimal */
            len = convert_int_to_char((int)ptr, 16, buff);
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

            written += (int)str_len;
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
