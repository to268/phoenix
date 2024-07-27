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
#include <phoenix/libpsf.h>
#include <phoenix/serial.h>

// NOLINTBEGIN(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)
extern unsigned char _binary____lib_libpsf_font_psf_start;
extern unsigned char _binary____lib_libpsf_font_psf_end;
// NOLINTEND(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

NODISCARD PSF1_font psf1_init(void) {
    PSF1_font font;

    auto* volatile header = (PSF1_header*)&_binary____lib_libpsf_font_psf_start;
    font.header = header;
    font.data = (u8*)header + sizeof(PSF1_header);

    /* Check the header */
    if (!PSF1_MAGIC_OK(header->magic))
        panic("Invalid PSF font !");

    if (header->mode & PSF1_MODE512)
        serial_writestring(SERIAL_COM1, "[PSF] Font with 512 characters\n");

    if (header->mode & PSF1_MODEHASTAB)
        serial_writestring(SERIAL_COM1, "[PSF] Font with unicode characters\n");

    serial_writestring(SERIAL_COM1, "[PSF] Loaded font\n");
    return font;
}
