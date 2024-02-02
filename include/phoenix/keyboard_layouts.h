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
#ifndef _KEYBOARD_LAYOUTS_H_
#define _KEYBOARD_LAYOUTS_H_

#ifdef __cplusplus
extern "C" {
#endif

static unsigned char keyboard_map_lowercase[0xff] = {
#if defined(KEYBOARD_AZERTY)
    0,    '\0', '&',  'é',  '\"', '\'', '(',  '-',  'è',  '_',  'ç',  'à',
    ')',  '=',  '\0', '\t', 'a',  'z',  'e',  'r',  't',  'y',  'u',  'i',
    'o',  'p',  '^',  '$',  '\n', '\0', 'q',  's',  'd',  'f',  'g',  'h',
    'j',  'k',  'l',  'm',  'ù',  '²',  '\0', '*',  'w',  'x',  'c',  'v',
    'b',  'n',  ',',  ';',  ':',  '!',  '\0', '\0', '\0', ' ',  '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.',
    0,    0,    0,    '\0', '\0', 0,    0,    0,
#elif defined(KEYBOARD_DVORAK)
    0,    '\0', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
    '[',  ']',  '\0', '\t', '\'', ',',  '.',  'p',  'y',  'f',  'g',  'c',
    'r',  'l',  '/',  '=',  '\n', '\0', 'a',  'o',  'e',  'u',  'i',  'd',
    'h',  't',  'n',  's',  '-',  '`',  '\0', '\\', ';',  'q',  'j',  'k',
    'x',  'b',  'm',  'w',  'v',  'z',  '\0', '\0', '\0', ' ',  '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.',
    0,    0,    0,    '\0', '\0', 0,    0,    0,
#else
    0,    '\0', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
    '-',  '=',  '\0', '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
    'o',  'p',  '[',  ']',  '\n', '\0', 'a',  's',  'd',  'f',  'g',  'h',
    'j',  'k',  'l',  ';',  '\'', '`',  '\0', '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  '\0', '\0', '\0', ' ',  '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.',
    0,    0,    0,    '\0', '\0', 0,    0,    0,
#endif
};

static unsigned char keyboard_map_uppercase[0xff] = {
#if defined(KEYBOARD_AZERTY)
    0,    '\0', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
    '°',  '+',  '\0', '\t', 'A',  'Z',  'E',  'R',  'T',  'Y',  'U',  'I',
    'O',  'P',  '¨',  '$',  '\n', '\0', 'Q',  'S',  'D',  'F',  'G',  'H',
    'J',  'K',  'L',  'M',  '%',  '~',  '\0', 'µ',  'W',  'X',  'C',  'V',
    'B',  'N',  '?',  '.',  '/',  '§',  '\0', '\0', '\0', ' ',  '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.',
    0,    0,    0,    '\0', '\0', 0,    0,    0,
#elif defined(KEYBOARD_DVORAK)
    0,    '\0', '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
    '{',  '}',  '\0', '\t', '\"', '<',  '>',  'P',  'Y',  'F',  'G',  'C',
    'R',  'L',  '?',  '+',  '\n', '\0', 'A',  'O',  'E',  'U',  'I',  'D',
    'H',  'T',  'N',  'S',  '_',  '~',  '\0', '|',  ':',  'Q',  'J',  'K',
    'X',  'B',  'M',  'W',  'V',  'Z',  '\0', '\0', '\0', ' ',  '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.',
    0,    0,    0,    '\0', '\0', 0,    0,    0,
#else
    0,    '\0', '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
    '_',  '+',  '\0', '\t', 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
    'O',  'P',  '{',  '}',  '\n', '\0', 'A',  'S',  'D',  'F',  'G',  'H',
    'J',  'K',  'L',  ':',  '\"', '~',  '\0', '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0', '\0', ' ',  '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.',
    0,    0,    0,    '\0', '\0', 0,    0,    0,
#endif
};

#ifdef __cplusplus
}
#endif

#endif /* _KEYBOARD_LAYOUTS_H_*/
