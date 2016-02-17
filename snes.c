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

#include "snes.h"
#include "util.h"

void snes_init(void) {
	bit_set(DDRB, _BV(PB0));
	bit_set(DDRB, _BV(PB1));
	bit_set(DDRB, _BV(PB2));
	bit_set(DDRB, _BV(PB3));
	bit_set(DDRB, _BV(PB4));
	bit_set(DDRB, _BV(PB5));
	bit_set(DDRC, _BV(PC0));
	bit_set(DDRC, _BV(PC1));
	bit_set(DDRC, _BV(PC2));
	bit_set(DDRC, _BV(PC3));
	bit_set(DDRD, _BV(PD3));
	bit_set(DDRD, _BV(PD7));
	bit_set(DDRD, _BV(PD0));
	bit_set(DDRD, _BV(PD1));
	bit_set(DDRD, _BV(PD2));
	bit_set(DDRD, _BV(PD4));

	snes_reset_output();
}

void snes_reset_output(void) {
	bit_set(PORTB, _BV(PB0));
	bit_set(PORTB, _BV(PB1));
	bit_set(PORTB, _BV(PB2));
	bit_set(PORTB, _BV(PB3));
	bit_set(PORTB, _BV(PB4));
	bit_set(PORTB, _BV(PB5));
	bit_set(PORTC, _BV(PC0));
	bit_set(PORTC, _BV(PC1));
	bit_set(PORTC, _BV(PC2));
	bit_set(PORTC, _BV(PC3));
	bit_set(PORTD, _BV(PD3));
	bit_set(PORTD, _BV(PD7));
	bit_set(PORTD, _BV(PD0));
	bit_set(PORTD, _BV(PD1));
	bit_set(PORTD, _BV(PD2));
	bit_set(PORTD, _BV(PD4));
}

void snes_press(uint8_t button) {
	switch(button) {
		case SNES_B:
			bit_clear(PORTB, _BV(PB0));
			break;
		case SNES_Y:
			bit_clear(PORTB, _BV(PB1));
			break;
		case SNES_SELECT:
			bit_clear(PORTB, _BV(PB2));
			break;
		case SNES_START:
			bit_clear(PORTB, _BV(PB3));
			break;
		case SNES_UP:
			snes_release(SNES_DOWN);
			bit_clear(PORTB, _BV(PB4));
			break;
		case SNES_DOWN:
			snes_release(SNES_UP);
			bit_clear(PORTB, _BV(PB5));
			break;
		case SNES_LEFT:
			snes_release(SNES_RIGHT);
			bit_clear(PORTC, _BV(PC0));
			break;
		case SNES_RIGHT:
			snes_release(SNES_LEFT);
			bit_clear(PORTC, _BV(PC1));
			break;
		case SNES_A:
			bit_clear(PORTC, _BV(PC2));
			break;
		case SNES_X:
			bit_clear(PORTC, _BV(PC3));
			break;
		case SNES_L:
			bit_clear(PORTD, _BV(PD3));
			break;
		case SNES_R:
			bit_clear(PORTD, _BV(PD7));
			break;
		case SNES_XTRA_1:
			bit_clear(PORTD, _BV(PD0));
			break;
		case SNES_XTRA_2:
			bit_clear(PORTD, _BV(PD1));
			break;
		case SNES_XTRA_3:
			bit_clear(PORTD, _BV(PD2));
			break;
		case SNES_XTRA_4:
			bit_clear(PORTD, _BV(PD4));
			break;
	}
}

void snes_release(uint8_t button) {
	switch(button) {
		case SNES_B:
			bit_set(PORTB, _BV(PB0));
			break;
		case SNES_Y:
			bit_set(PORTB, _BV(PB1));
			break;
		case SNES_SELECT:
			bit_set(PORTB, _BV(PB2));
			break;
		case SNES_START:
			bit_set(PORTB, _BV(PB3));
			break;
		case SNES_UP:
			bit_set(PORTB, _BV(PB4));
			break;
		case SNES_DOWN:
			bit_set(PORTB, _BV(PB5));
			break;
		case SNES_LEFT:
			bit_set(PORTC, _BV(PC0));
			break;
		case SNES_RIGHT:
			bit_set(PORTC, _BV(PC1));
			break;
		case SNES_A:
			bit_set(PORTC, _BV(PC2));
			break;
		case SNES_X:
			bit_set(PORTC, _BV(PC3));
			break;
		case SNES_L:
			bit_set(PORTD, _BV(PD3));
			break;
		case SNES_R:
			bit_set(PORTD, _BV(PD7));
			break;
		case SNES_XTRA_1:
			bit_set(PORTD, _BV(PD0));
			break;
		case SNES_XTRA_2:
			bit_set(PORTD, _BV(PD1));
			break;
		case SNES_XTRA_3:
			bit_set(PORTD, _BV(PD2));
			break;
		case SNES_XTRA_4:
			bit_set(PORTD, _BV(PD4));
			break;
	}
}
