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

#include <util/delay.h>
#include <arduino_serial.h>

#include "kbd.h"
#include "kbdhandler.h"
#include "ringbuffer.h"
#include "snes.h"

uint8_t str_buffer[255];


int main(void)
{
	snes_init();

	kbd_init();

	Serial_begin(57600);

	sprintf(str_buffer, "Reading from keyboard...\n");
	Serial_println(str_buffer);

	for(;;) {
		if(rb_hasitem()) {
			uint8_t key = rb_get();

			//keyboard_handle_key(key);

			sprintf(str_buffer, "Retrieved key code byte: %02X\n", key);
			Serial_println(str_buffer);
		} else {
			sprintf(str_buffer, "Idle...\n");
			Serial_println(str_buffer);
		}

		_delay_ms(500);
	}

	return 0;
}

