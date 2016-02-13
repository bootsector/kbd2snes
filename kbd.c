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

#include <avr/io.h>
#include <avr/interrupt.h>

#include "kbd.h"
#include "ringbuffer.h"
#include "util.h"

volatile static uint8_t bit = 0;
volatile static uint8_t data = 0;

void kbd_init(void) {
	bit_clear(DDRC, _BV(PC4));
	bit_set(PORTC, _BV(PC4));

	bit_clear(DDRC, _BV(PC5));
	bit_set(PORTC, _BV(PC5));

	bit = data = 0;

	bit_set(PCICR, _BV(PCIE1));
	bit_set(PCMSK1, _BV(PCINT12));

	sei();
}

ISR(PCINT1_vect)
{
	if(bit_is_clear(PINC, PC4)) {
		if(bit_is_set(PINC, PC5) && (bit > 0 && bit < 9)) {
			bit_set(data, _BV(bit-1));
		}

		bit++;

		if(bit > 10){
			rb_add(data);
			bit = data = 0;
		}
	}
}
