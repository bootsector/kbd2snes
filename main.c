#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "kbd.h"
#include "snes.h"

#include <arduino_serial.h>

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
			uint8_t item = rb_get();

			sprintf(str_buffer, "Retrieved item: %02X\n", item);
			Serial_println(str_buffer);
		} else {
			sprintf(str_buffer, "Idle...\n");
			Serial_println(str_buffer);
		}

		_delay_ms(1000);
	}

	return 0;
}

