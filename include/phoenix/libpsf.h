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
#ifndef _LIBPSF_H_
#define _LIBPSF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <phoenix/kernel.h>

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04
#define PSF1_MAXMODE    0x05

#define PSF1_SEPARATOR 0xFFFF
#define PSF1_STARTSEQ  0xFFFE

#define PSF1_MAGIC_OK(x) ((x)[0] == PSF1_MAGIC0 && (x)[1] == PSF1_MAGIC1)

typedef struct {
    unsigned char magic[2]; /* Magic number */
    u8 mode;                /* PSF font mode */
    u8 charsize;            /* Character size */
} PSF1_header;

typedef struct {
    PSF1_header* header;
    u8* data; /* Glyphs data */
} PSF1_font;

PSF1_font psf1_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBPSF_H_ */
