#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Serial Ports */
#define SERIAL_COM1 0x3f8
#define SERIAL_COM2 0x2f8
#define SERIAL_COM3 0x3e8
#define SERIAL_COM4 0x2e8

#define debug(string) serial_writestring(SERIAL_COM1, string)

/* Serial Functions */
int serial_init(const uint16_t serial_port, uint16_t baud_divisor);
int serial_port_check(const uint16_t serial_port);
int serial_received(const uint16_t serial_port);
char serial_read(const uint16_t serial_port);
int serial_is_transmit_empty(const uint16_t serial_port);
void serial_write(const uint16_t serial_port, const char c);
void serial_writestring(const uint16_t serial_port, const char* string);

#ifdef __cplusplus
}
#endif

#endif /*_SERIAL_H_*/
