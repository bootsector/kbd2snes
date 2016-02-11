#include <avr/io.h>

#include "snes.h"

void snes_init(void) {
	DDRB |= _BV(PB0);
	PORTB |= _BV(PB0);

	DDRB |= _BV(PB1);
	PORTB |= _BV(PB1);

	DDRB |= _BV(PB2);
	PORTB |= _BV(PB2);

	DDRB |= _BV(PB3);
	PORTB |= _BV(PB3);

	DDRB |= _BV(PB4);
	PORTB |= _BV(PB4);

	DDRB |= _BV(PB5);
	PORTB |= _BV(PB5);

	DDRC |= _BV(PC0);
	PORTC |= _BV(PC0);

	DDRC |= _BV(PC1);
	PORTC |= _BV(PC1);

	DDRC |= _BV(PC2);
	PORTC |= _BV(PC2);

	DDRC |= _BV(PC3);
	PORTC |= _BV(PC3);

	DDRD |= _BV(PD3);
	PORTD |= _BV(PD3);

	DDRD |= _BV(PD7);
	PORTD |= _BV(PD7);
}

void snes_press(uint8_t button) {
	switch(button) {
		case SNES_B:
			PORTB &= ~(_BV(PB0));
			break;
		case SNES_Y:
			PORTB &= ~(_BV(PB1));
			break;
		case SNES_SELECT:
			PORTB &= ~(_BV(PB2));
			break;
		case SNES_START:
			PORTB &= ~(_BV(PB3));
			break;
		case SNES_UP:
			snes_release(SNES_DOWN);
			PORTB &= ~(_BV(PB4));
			break;
		case SNES_DOWN:
			snes_release(SNES_UP);
			PORTB &= ~(_BV(PB5));
			break;
		case SNES_LEFT:
			snes_release(SNES_RIGHT);
			PORTC &= ~(_BV(PC0));
			break;
		case SNES_RIGHT:
			snes_release(SNES_LEFT);
			PORTC &= ~(_BV(PC1));
			break;
		case SNES_A:
			PORTC &= ~(_BV(PC2));
			break;
		case SNES_X:
			PORTC &= ~(_BV(PC3));
			break;
		case SNES_L:
			PORTD &= ~(_BV(PD3));
			break;
		case SNES_R:
			PORTD &= ~(_BV(PD7));
			break;
	}
}

void snes_release(uint8_t button) {
	switch(button) {
		case SNES_B:
			PORTB |= _BV(PB0);
			break;
		case SNES_Y:
			PORTB |= _BV(PB1);
			break;
		case SNES_SELECT:
			PORTB |= _BV(PB2);
			break;
		case SNES_START:
			PORTB |= _BV(PB3);
			break;
		case SNES_UP:
			PORTB |= _BV(PB4);
			break;
		case SNES_DOWN:
			PORTB |= _BV(PB5);
			break;
		case SNES_LEFT:
			PORTC |= _BV(PC0);
			break;
		case SNES_RIGHT:
			PORTC |= _BV(PC1);
			break;
		case SNES_A:
			PORTC |= _BV(PC2);
			break;
		case SNES_X:
			PORTC |= _BV(PC3);
			break;
		case SNES_L:
			PORTD |= _BV(PD3);
			break;
		case SNES_R:
			PORTD |= _BV(PD7);
			break;
	}
}
