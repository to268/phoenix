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
#ifndef _NMI_H_
#define _NMI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

/* System control port A flags */
#define NMI_ALTERNATE_HOT_RESET     (1 << 0)
#define NMI_ALTERNATE_GATE_A20      (1 << 1)
#define NMI_SECURITY_LOCK           (1 << 3)
#define NMI_WATCHDOG_TIMER_STATUS   (1 << 4)
#define NMI_HDD_1_DRIVE_ACTIVITY    (1 << 6)
#define NMI_HDD_2_DRIVE_ACTIVITY    (1 << 7)

/* System control port B flags */
#define NMI_TIMER_2_TIED_TO_SPEAKER (1 << 8)
#define NMI_SPEAKER_DATA_ENABLE     (1 << 9)
#define NMI_PARITY_CHECK_ENABLE     (1 << 10)
#define NMI_CHANNEL_CHECK_ENABLE    (1 << 11)
#define NMI_REFRESH_REQUEST         (1 << 12)
#define NMI_TIMER_2_OUTPUT          (1 << 13)
#define NMI_CHANNEL_CHECK           (1 << 14)
#define NMI_PARITY_CHECK            (1 << 15)

void nmi_enable(void);
void nmi_disable(void);
u16 nmi_get_control_ports(void);

#ifdef __cplusplus
}
#endif

#endif /* _NMI_H_*/
