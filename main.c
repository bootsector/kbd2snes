#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "kbd.h"
#include "snes.h"

#include <arduino_serial.h>

uint8_t str_buffer[255];

/*
Suggested PS/2 Keyboard to SNES conversion:

Key		Make	Break		SNES Out
W		1D		F0,1D		UP
A		1C		F0,1C		LEFT
S		1B		F0,1B		DOWN
D		23		F0,23		RIGHT
I		43		F0,43		UP
J		3B		F0,3B		LEFT
K		42		F0,42		DOWN
L		4B		F0,4B		RIGHT
U ARROW	E0,75	E0,F0,75	UP
L ARROW	E0,6B	E0,F0,6B	LEFT
D ARROW	E0,72	E0,F0,72	DOWN
R ARROW	E0,74	E0,F0,74	RIGHT
KP 1	69		F0,69		Y
KP 2	72		F0,72		B
KP 4	6B		F0,6B		X
KP 5	73		F0,73		A
KP 3	7A		F0,7A		L
KP 6	74		F0,74		R
L SHFT	12		FO,12		SELECT
L CTRL	14		FO,14		START
*/

static int kbd_extended_flag = 0;

void keyboard_ignore_next_keys(int keys) {
	while(keys--) {
		while(!rb_hasitem());

		rb_get();
	}
}

void keyboard_make(uint8_t key) {
	if(kbd_extended_flag) {
		switch(key) {
			case 0x12: // PRNT SCRN (Make) E0,12,E0,7C
				keyboard_ignore_next_keys(2);
				break;
			case 0x75: // U ARROW
				snes_press(SNES_UP);
				break;
			case 0x6B: // L ARROW
				snes_press(SNES_LEFT);
				break;
			case 0x72: // D ARROW
				snes_press(SNES_DOWN);
				break;
			case 0x74: // R ARROW
				snes_press(SNES_RIGHT);
				break;
		}
	} else {
		switch(key) {
			case 0x1D: // W
				snes_press(SNES_UP);
				break;
			case 0x1C: // A
				snes_press(SNES_LEFT);
				break;
			case 0x1B: // S
				snes_press(SNES_DOWN);
				break;
			case 0x23: // D
				snes_press(SNES_RIGHT);
				break;
			case 0x43: // I
				snes_press(SNES_UP);
				break;
			case 0x3B: // J
				snes_press(SNES_LEFT);
				break;
			case 0x42: // K
				snes_press(SNES_DOWN);
				break;
			case 0x4B: // L
				snes_press(SNES_RIGHT);
				break;
			case 0x69: // KP 1
				snes_press(SNES_Y);
				break;
			case 0x72: // KP 2
				snes_press(SNES_B);
				break;
			case 0x6B: // KP 4
				snes_press(SNES_X);
				break;
			case 0x73: // KP 5
				snes_press(SNES_A);
				break;
			case 0x7A: // KP 3
				snes_press(SNES_L);
				break;
			case 0x74: // KP 6
				snes_press(SNES_R);
				break;
			case 0x12: // L SHFT
				snes_press(SNES_SELECT);
				break;
			case 0x14: // L CTRL
				snes_press(SNES_START);
				break;
		}
	}
}

void keyboard_break(void) {
	while(!rb_hasitem());

	uint8_t key = rb_get();

	if(kbd_extended_flag) {
		switch(key) {
			case 0x7C: // PRNT SCRN (Break) E0,F0,7C,E0,F0,12
				keyboard_ignore_next_keys(3);
				break;
			case 0x75: // U ARROW
				snes_release(SNES_UP);
				break;
			case 0x6B: // L ARROW
				snes_release(SNES_LEFT);
				break;
			case 0x72: // D ARROW
				snes_release(SNES_DOWN);
				break;
			case 0x74: // R ARROW
				snes_release(SNES_RIGHT);
				break;
		}

	} else {
		switch(key) {
			case 0x1D: // W
				snes_release(SNES_UP);
				break;
			case 0x1C: // A
				snes_release(SNES_LEFT);
				break;
			case 0x1B: // S
				snes_release(SNES_DOWN);
				break;
			case 0x23: // D
				snes_release(SNES_RIGHT);
				break;
			case 0x43: // I
				snes_release(SNES_UP);
				break;
			case 0x3B: // J
				snes_release(SNES_LEFT);
				break;
			case 0x42: // K
				snes_release(SNES_DOWN);
				break;
			case 0x4B: // L
				snes_release(SNES_RIGHT);
				break;
			case 0x69: // KP 1
				snes_release(SNES_Y);
				break;
			case 0x72: // KP 2
				snes_release(SNES_B);
				break;
			case 0x6B: // KP 4
				snes_release(SNES_X);
				break;
			case 0x73: // KP 5
				snes_release(SNES_A);
				break;
			case 0x7A: // KP 3
				snes_release(SNES_L);
				break;
			case 0x74: // KP 6
				snes_release(SNES_R);
				break;
			case 0x12: // L SHFT
				snes_release(SNES_SELECT);
				break;
			case 0x14: // L CTRL
				snes_release(SNES_START);
				break;
		}
	}
}

void keyboard_extended(void) {
	while(!rb_hasitem());

	kbd_extended_flag = 1;

	uint8_t key = rb_get();

	switch(key) {
		case 0xF0:
			keyboard_break();
			break;
		default:
			keyboard_make(key);
			break;
	}
}

void keyboard_handle_key(uint8_t key) {
	kbd_extended_flag = 0;

	switch(key) {
		case 0xF0:
			keyboard_break();
			break;
		case 0xE0:
			keyboard_extended();
			break;
		case 0xE1: // PAUSE (Make) E1,14,77,E1,F0,14,F0,77
			keyboard_ignore_next_keys(7);
			break;
		default:
			keyboard_make(key);
			break;
	}
}

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

			sprintf(str_buffer, "Retrieved key code: %02X\n", key);
			Serial_println(str_buffer);
		} else {
			sprintf(str_buffer, "Idle...\n");
			Serial_println(str_buffer);
		}

		_delay_ms(1000);
	}

	return 0;
}

