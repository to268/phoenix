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
#ifndef _RTC_H_
#define _RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

#define RT_FIRST_YEAR_OF_CENTURY 2000

struct time {
    u32 hour;
    u32 second;
    u32 minute;
};

struct date_time {
    u32 day;
    u32 month;
    u32 year;
    struct time time;
};

void rtc_init(void);
void rtc_update_date_time(void);
NODISCARD struct date_time* rtc_get_date_time(void);
void rtc_print_date_time(void);

#ifdef __cplusplus
}
#endif

#endif /* _RTC_H_*/
