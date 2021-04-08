#include <phoenix/kernel.h>
#include <stdint.h>

int puts(uint8_t severity, const char* string);

int puts(uint8_t severity, const char* string) {
	return printk(severity, "%s\n", string);
}
