#include <phoenix/kernel.h>
#include <phoenix/mem.h>
#include <phoenix/vga.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define EOF (-1)
#define INT_MAX 2147483647

static int print(const char* data, size_t length, uint8_t severity);
int printk(uint8_t severity, const char* restrict format, ...);

static int print(const char* data, size_t length, uint8_t severity) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i], severity) == EOF)
			return 0;
	return 1;
}

int printk(uint8_t severity, const char* restrict format, ...) {
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
                return -1;
            }
            if (!print(format, amount, severity))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        switch (*format) {
            case 'c':
            case 'C':
                format++;
                char c = (char) va_arg(parameters, int /* char promotes to int */);
                if (!maxrem) {
                    return -1;
                }
                if (!print(&c, sizeof(c), severity))
                    return -1;
                written++;
                break;

            case 's':
            case 'S':
                format++;
                const char* str = va_arg(parameters, const char*);
                size_t str_len = strlen(str);
                if (maxrem < str_len) {
                    return -1;
                }
                if (!print(str, str_len, severity))
                    return -1;
                written += str_len;
                break;

            case 'b':
            case 'B':
                format++;
                int number = va_arg(parameters, int);
                char buff[8];
                // convert char to binary
                itoa(number, buff, 2);
                int len = strlen(buff);
                if (!print(buff, len, severity))
                    return -1;
                written += len;
                break;

            case 'd':
            case 'D':
                format++;
                number = va_arg(parameters, int);
                // convert char to int
                itoa(number, buff, 10);
                len = strlen(buff);
                if (!print(buff, len, severity))
                    return -1;
                written += len;
                break;

            case 'o':
            case 'O':
                format++;
                number = va_arg(parameters, int);
                // convert char to octal
                itoa(number, buff, 8);
                len = strlen(buff);
                if (!print(buff, len, severity))
                    return -1;
                written += len;
                break;

            case 'x':
            case 'X':
                format++;
                number = va_arg(parameters, int);
                // convert char to hex
                itoa(number, buff, 16);
                len = strlen(buff);
                if (!print(buff, len, severity))
                    return -1;
                written += len;
                break;

            default:
                format = format_begun_at;
                str_len = strlen(format);
                if (maxrem < str_len) {
                    return -1;
                }
                if (!print(format, str_len, severity))
                    return -1;
                written += str_len;
                format += str_len;
                break;
        }

    }

    va_end(parameters);
    return written;
}
