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
#include <phoenix/io.h>

ALWAYS_INLINE void tests_check(bool result);

void tests_main(void) {
    info("Running tests..\n\n");

    extern bool test_sse(void);
    info("test_sse: ");
    tests_check(test_sse());

    info("\nDONE!\n");
    /* Clear interrupts and Halt CPU */
    cli();
    NORETURN extern void halt_cpu(void);
    halt_cpu();
}

ALWAYS_INLINE void tests_check(bool success) {
    if (success) {
        valid(" PASS!\n");
        return;
    }

    error(" FAIL!\n");
}
