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
#include <phoenix/pic.h>
#include <phoenix/nmi.h>
#include <phoenix/rtc.h>
#include <phoenix/io.h>

static struct date_time date;

void rtc_init(void) {
    /* Disable interrupts and NMIs */
    nmi_disable();
    cli();

    /* Write to register A */
    outb(0x70, 0x8a);
    outb(0x71, 0x20);

    /* Enable interrupts and NMIs */
    sti();
    nmi_enable();

    /* Make an initial update */
    rtc_update_date_time();
}

NODISCARD static u8 rtc_is_updating() {
    outb(0x70, 0x0a);
    return inb(0x71) & 0x80;
}

NODISCARD static u8 rtc_read(u8 reg) {
    while (rtc_is_updating())
        ;

    outb(0x70, reg);
    return inb(0x71);
}

void rtc_update_date_time(void) {
    /* Read date */
    auto day = rtc_read(0x7);
    auto month = rtc_read(0x8);
    auto year = rtc_read(0x9);

    /* Convert BCD date to binary */
    date.day = (day & 0xf) + ((day / 16) * 10);
    date.month = (month & 0xf) + ((month / 16) * 10);
    date.year = (year & 0xf) + ((year / 16) * 10);
    date.year += RT_FIRST_YEAR_OF_CENTURY;

    /* Read time */
    u8 hours = rtc_read(0x4);
    u8 minutes = rtc_read(0x2);
    u8 seconds = rtc_read(0);

    /* Convert BCD time to binary */
    date.time.hour =
        ((hours & 0xf) + (((hours & 0x70) / 16) * 10)) | (hours & 0x80);
    date.time.minute = (minutes & 0xf) + ((minutes / 16) * 10);
    date.time.second = (seconds & 0xf) + ((seconds / 16) * 10);
}

NODISCARD RETURNS_NONNULL struct date_time* rtc_get_date_time(void) {
    return &date;
}

void rtc_print_date_time(void) {
    /* Make sure to update the time */
    rtc_update_date_time();

    info("date: %d/%d/%d\n", date.day, date.month, date.year);
    info("time: %d hour(s) %d minute(s) %d second(s) UTC\n", date.time.hour,
         date.time.minute, date.time.second);
}
