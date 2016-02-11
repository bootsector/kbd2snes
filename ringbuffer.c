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

#include "ringbuffer.h"

#define BUFFER_SIZE 128

static uint8_t buffer[BUFFER_SIZE];

static volatile int head = 0;
static volatile int tail = 0;

int rb_getcount() {
	return (BUFFER_SIZE + head - tail) % BUFFER_SIZE;
}

uint8_t rb_hasitem(void) {
	return rb_getcount() > 0;
}

void rb_add(uint8_t item) {
	buffer[head] = item;

	head = (head + 1) % BUFFER_SIZE;

	// Head pushes Tail forward if full...
	if (head == tail) {
		tail = (tail + 1) % BUFFER_SIZE;
	}
}

uint8_t rb_get() {
	uint8_t element = 0;

	if(rb_hasitem()) {
		element = buffer[tail];
		tail = (tail + 1) % BUFFER_SIZE;
	}

	return element;
}

