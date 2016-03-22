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
#include "util.h"

#define BUFFER_SIZE 128

static uint8_t buffer[BUFFER_SIZE];

static volatile int head = 0;
static volatile int tail = 0;

volatile static uint8_t bit = 0;
volatile static uint8_t data = 0;

// Count time frames of 32.78ms (for 8Mhz clock).
volatile static uint8_t time_counter = 0;

void kbd_init(void) {
	// Pullup check pin (D5) as input (pullup on)
	bit_clear(DDRD, _BV(PD5));
	bit_set(PORTD, _BV(PD5));

	// Clock pin as input
	bit_clear(DDRC, _BV(PC4));

	// Data pin as input
	bit_clear(DDRC, _BV(PC5));

	// If pullups switch is in ON position (grounded), activates pullups for data and clock lines
	if(bit_is_clear(PIND, PD5)) {
		bit_set(PORTC, _BV(PC4));
		bit_set(PORTC, _BV(PC5));
	}

	bit = data = time_counter = 0;

	// Clock pin change interrupt
	bit_set(PCICR, _BV(PCIE1));
	bit_set(PCMSK1, _BV(PCINT12));

	// Timer0 overflow interrupt setup: Prescaler = FCPU/1024
	TCCR0A = 0;
	TCCR0B |= (1 << CS02) | (1 << CS00);

	TCNT0 = 0;

	// enable Timer0 overflow interrupt:
	TIMSK0 = (1 << TOIE0);

	sei();
}

int kbd_rb_getcount() {
	return (BUFFER_SIZE + head - tail) % BUFFER_SIZE;
}

uint8_t kbd_rb_hasitem(void) {
	return kbd_rb_getcount() > 0;
}

void kbd_rb_add(uint8_t item) {
	buffer[head] = item;

	head = (head + 1) % BUFFER_SIZE;

	// Head pushes Tail forward if full...
	if (head == tail) {
		tail = (tail + 1) % BUFFER_SIZE;
	}
}

uint8_t kbd_rb_get() {
	uint8_t element = 0;

	if(kbd_rb_hasitem()) {
		element = buffer[tail];
		tail = (tail + 1) % BUFFER_SIZE;
	}

	return element;
}

// Clock line change interrupt handler
ISR(PCINT1_vect)
{
	if(bit_is_clear(PINC, PC4)) {
		// More than ~250ms has passed since last clock low signal. Reset serial data variables.
		if(time_counter > 7) {
			bit = data = 0;
		}

		if(bit_is_set(PINC, PC5) && (bit > 0 && bit < 9)) {
			bit_set(data, _BV(bit-1));
		}

		bit++;

		if(bit > 10){
			kbd_rb_add(data);
			bit = data = 0;
		}

		time_counter = 0;
	}
}

// This will be called around 30.5 times within a second, or at every 32.78ms (for a 8Mhz clock)
ISR(TIMER0_OVF_vect)
{
	if(time_counter < 0xFF) {
		time_counter++;
	}
}
