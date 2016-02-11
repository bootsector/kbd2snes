#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "kbd.h"
#include "snes.h"

#include <arduino_serial.h>

uint8_t str_buffer[255];

/*
Suggested PS/2 Keyboard to SNES conversion:

Key		Make	Break	SNES Out
W		1D		F0,1D	UP
A		1C		F0,1C	LEFT
S		1B		F0,1B	DOWN
D		23		F0,23	RIGHT
J		3B		F0,3B	LEFT
K		42		F0,42	DOWN
L		4B		F0,4B	RIGHT
I		43		F0,43	UP
KP 1	69		F0,69	Y
KP 2	72		F0,72	B
KP 4	6B		F0,6B	X
KP 5	73		F0,73	A
KP 3	7A		F0,7A	L
KP 6	74		F0,74	R
L SHFT	12		FO,12	SELECT
L CTRL	14		FO,14	START
*/

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

