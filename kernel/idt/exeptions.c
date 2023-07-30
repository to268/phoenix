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
#include <phoenix/kernel.h>
#include <phoenix/serial.h>
#include <phoenix/idt.h>
#include <phoenix/nmi.h>
#include <phoenix/io.h>

/* List of exceptions messages */
static const char* exceptions[] = {
    [0] = "Division by Zero",
    [1] = "Debug",
    [2] = "Non Maskable Interrupt",
    [3] = "Breakpoint",
    [4] = "Overflow",
    [5] = "Bound Range Exceeded",
    [6] = "Invalid opcode",
    [7] = "Device not available",
    [8] = "Double Fault",
    [10] = "Invalid TSS",
    [11] = "Segment not present",
    [12] = "Stack Exception",
    [13] = "General Protection fault",
    [14] = "Page fault",
    [16] = "x87 Floating Point Exception",
    [17] = "Alignment check",
    [18] = "Machine check",
    [19] = "SIMD floating point Exception",
    [20] = "Virtualization Exception",
    [30] = "Security Exception",
};

void handle_nmi(void);

NORETURN
void exception_handler(u64 vector) {
    cli();

    /* Handle the exception vector */
    switch (vector) {
    case 2:
        handle_nmi();
        break;
    }

    panic(exceptions[vector]);
}

void handle_nmi(void) {
    warn("NMI interrupt fired !\n");
    serial_writestring(SERIAL_COM1, "[IVT] NMI interrupt fired !\n");

    u16 control_ports = nmi_get_control_ports();

    /* Check channel */
    if (control_ports & NMI_CHANNEL_CHECK_ENABLE &&
        control_ports & NMI_CHANNEL_CHECK) {
        error("NMI failure on the bus probably caused by a peripheral device "
              "!\n");
        serial_writestring(SERIAL_COM1, "[IVT] NMI failure on the bus probably "
                                        "caused by a peripheral device !\n");
    }

    /* Check parity */
    if (control_ports & NMI_PARITY_CHECK_ENABLE &&
        control_ports & NMI_PARITY_CHECK) {
        error("NMI memory read or write failure !\n");
        serial_writestring(SERIAL_COM1,
                           "[IVT] NMI memory read or write failure !\n");
    }

    /* Check watchdog */
    if (control_ports & NMI_WATCHDOG_TIMER_STATUS) {
        error("NMI watchdog timer, kernel lock up !\n");
        serial_writestring(SERIAL_COM1,
                           "[IVT] NMI watchdog timer, kernel lock up !\n");
    }

    /* We don't handle the rest of the NMIs for now
     * so we dump the control ports values
     */
    error("NMI other error: 0b%b\n", control_ports);

    serial_writestring(SERIAL_COM1, "[IVT] NMI other error: 0b");

    /* Convert the control ports values as characters to the serial port */
    char control_ports_buff[17];
    convert_int_to_char(control_ports, 2, control_ports_buff);
    serial_writestring(SERIAL_COM1, control_ports_buff);
    serial_writestring(SERIAL_COM1, "\n");
}
