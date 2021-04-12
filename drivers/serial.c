#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/mem.h>
#include <phoenix/io.h>

int serial_init(const uint16_t serial_port, uint16_t baud_divisor);
int serial_port_check(const uint16_t serial_port);
int serial_received(const uint16_t serial_port);
char serial_read(const uint16_t serial_port);
int serial_is_transmit_empty(const uint16_t serial_port);
void serial_write(const uint16_t serial_port, const char c);
void serial_writestring(const uint16_t serial_port, const char* string);

static uint8_t serial_initialized = 0;

/* Initialize serial connexion */
int serial_init(const uint16_t serial_port, uint16_t baud_divisor)
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
    if(inb(serial_port + 0) != 0xae) {
        return 1;
    }

    /* If serial is working, set it in normal operation mode */
    outb(serial_port + 4, 0x0f);
    serial_initialized = 1;
    return 0;
}

/* Check if a serial port is valid */
int serial_port_check(const uint16_t serial_port)
{
    if (serial_port == SERIAL_COM1 || serial_port == SERIAL_COM2 ||
        serial_port == SERIAL_COM3 || serial_port == SERIAL_COM4) {
        return 0;
    }
    return 1;
}

/* Check if we can receive data */
int serial_received(const uint16_t serial_port)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return -1;

    return inb(serial_port + 5) & 1;
}

/* Read data thought serial */
char serial_read(const uint16_t serial_port)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return -1;

    while (serial_received(serial_port) == 0);

    return inb(serial_port);
}

/* Check the end of the received data */
int serial_is_transmit_empty(const uint16_t serial_port)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return -1;

    return inb(serial_port + 5) & 0x20;
}

/* Write a char thought serial */
void serial_write(const uint16_t serial_port, const char c)
{
    /* Check is the serial port is supported */
    if (!serial_initialized || serial_port_check(serial_port)) return;

    while (serial_is_transmit_empty(serial_port) == 0);

    outb(serial_port, c);
}

/* Write a string thought serial */
void serial_writestring(const uint16_t serial_port, const char* string)
{
    while (*string) {
        serial_write(serial_port, *(string++));
    }
}
