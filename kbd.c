#include <avr/io.h>
#include <avr/interrupt.h>

#include "kbd.h"
#include "ringbuffer.h"

void kbd_init(void) {
	DDRC &= ~(_BV(PC4));
	PORTC |= _BV(PC4);

	DDRC &= ~(_BV(PC5));
	PORTC |= _BV(PC5);

	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT12);

	sei();
}

static inline kbd_read(void) {
	uint8_t i;
	uint8_t data = 0;
	uint8_t in_time;

	for(i = 0; i < 10; i++) {
		// Wait for HIGH
		in_time = 0xFF;
		do {
			in_time--;
		} while(bit_is_clear(PINC, PC4) && in_time);

		if(!in_time) {
			break;
		}

		// Wait for LOW
		in_time = 0xFF;
		do {
			in_time--;
		} while(bit_is_set(PINC, PC4) && in_time);

		if(!in_time) {
			break;
		}

		// Read data bit
		if(bit_is_set(PINC, PC5) && i < 8) {
			data |= _BV(i);
		}
	}

	// i > 7 here means no timeout for the first 8 data bits...
	if(i > 7) {
		rb_add(data);
	}
}

ISR(PCINT1_vect)
{
	if(bit_is_clear(PINC, PC4)) {
		kbd_read();
	}
}