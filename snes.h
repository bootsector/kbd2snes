/*
 * KBD2SNES - PS/2 Keyboard Adapter for SNES
 * Copyright (c) 2016 Bruno Freitas - bruno@brunofreitas.com
 *
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

#ifndef SNES_H_
#define SNES_H_

#include <stdint.h>

#define SNES_B		0
#define SNES_Y		1
#define SNES_SELECT	2
#define SNES_START	3
#define SNES_UP		4
#define SNES_DOWN	5
#define SNES_LEFT	6
#define SNES_RIGHT	7
#define SNES_A		8
#define SNES_X		9
#define SNES_L		10
#define SNES_R		11
#define SNES_XTRA_1	12
#define SNES_XTRA_2	13
#define SNES_XTRA_3	14
#define SNES_XTRA_4	15

#define SNES_NO_ACTION	255

void snes_init(void);
void snes_press(uint8_t button);
void snes_release(uint8_t button);
void snes_reset_output(void);

#endif /* SNES_H_ */
