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
#include <phoenix/io.h>
#include <phoenix/pic.h>
#include <phoenix/pit.h>

static volatile u64 pit_timer_ticks = 0;

void pit_init(u32 hz) {
    /* Calculate the divisor */
    u32 divisor = PIT_FREQ / hz;

    /* Apply divisor */
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xff);
    outb(0x40, divisor >> 8);

    /* Unmask PIT IRQ */
    pic_irq_clear_mask(PIT_IRQ);
}

void pit_handler(void) {
    /* Increment ticks count when IRQ is triggered */
    pit_timer_ticks++;

    pic_send_eoi(PIT_IRQ);
}

inline u64 pit_get_ticks(void) { return pit_timer_ticks; }

void pit_sleep(u64 msec) {
    u64 ticks = pit_timer_ticks + msec;
    while (pit_timer_ticks < ticks)
        ;
}
