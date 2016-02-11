#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ringbuffer.h"

#include <arduino_serial.h>

uint8_t str_buffer[255];

#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))

int main(void)
{
	// Arduino's Serial library needs this
	sei(); // Enable global interrupts

	Serial_begin(57600);

	for(int i = 0; i < 5; i++) {
		rb_add(i);
	}

	while(1) {
		uint8_t item = rb_get();

		sprintf(str_buffer, "Retrieved item: %d\n", item);

		Serial_println(str_buffer);

		sprintf(str_buffer, "Size: %d\n", rb_getcount());

		Serial_println(str_buffer);

		_delay_ms(2000);
	}

	for(;;);

	return 0;
}

