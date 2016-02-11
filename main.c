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

