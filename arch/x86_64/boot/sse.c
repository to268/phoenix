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
#include <phoenix/cpu.h>
#include <phoenix/serial.h>

ALIGNED(16) static char fxsave_region[512];

void sse_init(void);
void sse_save_state(void);
void sse_restore_state(void);

void sse_init(void) {
    debug("[SSE] Initializing SSE\n");

    if (!cpu_check_feature_edx(CPUID_FEAT_EDX_SSE)) {
        debug("[SSE] No SSE support\n");
        return;
    }

    extern void sse_enable(void);
    sse_enable();

    /* Save Initial state */
    sse_save_state();
    debug("[SSE] Enabled SSE support\n");

    /* Enable OXSAVE is available */
    if (!cpu_check_feature_ecx(CPUID_FEAT_ECX_XSAVE)) {
        return;
    }

    extern void sse_enable_oxsave(void);
    sse_enable_oxsave();
    debug("[SSE] Enabled XSAVE support\n");

    /* Enable AVX is available */
    if (!cpu_check_feature_ecx(CPUID_FEAT_ECX_AVX)) {
        return;
    }

    extern void sse_enable_avx(void);
    sse_enable_avx();
    debug("[SSE] Enabled AVX support\n");
}

void sse_save_state(void) {
    __asm__ volatile("fxsave %0" ::"m"(fxsave_region));
}

void sse_restore_state(void) {
    __asm__ volatile("fxrstor %0" ::"m"(fxsave_region));
}
